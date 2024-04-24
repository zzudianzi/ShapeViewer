#include "pch.h"

#include "BoxApp.h"
#include "CommonStructure.h"

#include <DirectXColors.h>

#include <filesystem>

using namespace DirectX;
using namespace ShapeViewer;

BoxApp::BoxApp(winrt::com_ptr<ISwapChainPanelNative> panelNative) : D3DApp(panelNative)
{
}

BoxApp::~BoxApp() = default;

bool BoxApp::InitDirect3D()
{
    if (!D3DApp::InitDirect3D())
    {
        return false;
    }

    winrt::check_hresult(_CommandList->Reset(_DirectCmdListAlloc.get(), nullptr));

    BuildDescriptorHeaps();
    BuildConstantBuffers();
    BuildRootSignature();
    BuildShaderAndInputLayout();
    BuildBoxGeometry();
    BuildPSO();

    winrt::check_hresult(_CommandList->Close());
    ID3D12CommandList* cmdsList[] = {_CommandList.get()};
    _CommandQueue->ExecuteCommandLists(_countof(cmdsList), cmdsList);

    FlushCommandQueue();

    return true;
}

void BoxApp::OnResize(float width, float height)
{
    D3DApp::OnResize(width, height);

    XMMATRIX p = XMMatrixPerspectiveFovLH(0.25 * DirectX::XM_PI, AspectRatio(), 1., 1000.f);
    XMStoreFloat4x4(&_Proj, p);
}

void BoxApp::Update()
{
    // Convert Spherical to Cartesian coordinates.
    float x = _Radius * sin(_Phi) * cos(_Theta);
    float y = _Radius * cos(_Phi);
    float z = _Radius * sin(_Phi) * sin(_Theta);

    // Build the view matrix
    XMVECTOR pos = XMVectorSet(x, y, z, 1.f);
    XMVECTOR target = XMVectorZero();
    XMVECTOR up = XMVectorSet(0.f, 1.f, 0.f, 0.f);

    XMMATRIX view = XMMatrixLookAtLH(pos, target, up);
    XMStoreFloat4x4(&_View, view);

    XMMATRIX world = XMLoadFloat4x4(&_World);
    XMMATRIX proj = XMLoadFloat4x4(&_Proj);
    XMMATRIX worldViewProj = world * view * proj;

    // Update the constant buffer with the latest worldViewProj matrix.
    ObjectConstants objConstants;
    XMStoreFloat4x4(&objConstants._WorldViewProj, XMMatrixTranspose(worldViewProj));
    _ObjectCB->CopyData(0, objConstants);
}

void BoxApp::Draw()
{
    winrt::check_hresult(_DirectCmdListAlloc->Reset());

    winrt::check_hresult(_CommandList->Reset(_DirectCmdListAlloc.get(), _PSO.get()));

    _CommandList->RSSetViewports(1, &_ScreenViewport);
    _CommandList->RSSetScissorRects(1, &_ScissorRect);

    auto transitionRender = CD3DX12_RESOURCE_BARRIER::Transition(
        CurrentBackBuffer(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
    _CommandList->ResourceBarrier(1, &transitionRender);

    _CommandList->ClearRenderTargetView(CurrentBackBufferView(), DirectX::Colors::LightSteelBlue, 0, nullptr);
    _CommandList->ClearDepthStencilView(
        DepthStencilView(), D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.f, 0, 0, nullptr);

    auto currentBackBufferView = CurrentBackBufferView();
    auto depthStencilView = DepthStencilView();
    _CommandList->OMSetRenderTargets(1, &currentBackBufferView, true, &depthStencilView);

    ID3D12DescriptorHeap* descriptorHeaps[] = {_CBVHeap.get()};
    _CommandList->SetDescriptorHeaps(_countof(descriptorHeaps), descriptorHeaps);

    _CommandList->SetGraphicsRootSignature(_RootSignature.get());

    auto vertexBufferView = _BoxGeometry->VertexBufferView();
    auto indexBufferView = _BoxGeometry->IndexBufferView();
    _CommandList->IASetVertexBuffers(0, 1, &vertexBufferView);
    _CommandList->IASetIndexBuffer(&indexBufferView);
    _CommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINESTRIP);

    _CommandList->SetGraphicsRootDescriptorTable(0, _CBVHeap->GetGPUDescriptorHandleForHeapStart());

    _CommandList->DrawIndexedInstanced(_BoxGeometry->_DrawArgs["box"]._IndexCount, 1, 0, 0, 0);

   /* auto transitionPresent = CD3DX12_RESOURCE_BARRIER::Transition(
        CurrentBackBuffer(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);

    _CommandList->ResourceBarrier(1, &transitionPresent);*/

    winrt::check_hresult(_CommandList->Close());

    ID3D12CommandList* cmdsLists[] = {_CommandList.get()};
    _CommandQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);

    D3DApp::Draw2D();

    winrt::check_hresult(_SwapChain->Present(0, 0));
    _CurrentBackBuffer = (_CurrentBackBuffer + 1) % _SwapChainBufferCount;

    FlushCommandQueue();
}

void BoxApp::OnMouseDown(WPARAM btnState, int x, int y)
{
    _LastMousePos.x = x;
    _LastMousePos.y = y;
}

void BoxApp::OnMouseUp(WPARAM btnState, int x, int y)
{

}

void BoxApp::OnMouseMove(WPARAM btnState, int x, int y)
{
    if (btnState & MK_LBUTTON)
    {
        // Make each pixel correspond to a quarter of a degree.
        float dx = XMConvertToRadians(0.25f * static_cast<float>(x - _LastMousePos.x));
        float dy = XMConvertToRadians(0.25f * static_cast<float>(y - _LastMousePos.y));

        // Update angles based on input to orbit camera around box.
        _Theta += dx;
        _Phi += dy;

        // Restrict the angle _Phi
        _Phi = std::clamp(_Phi, 1.f, DirectX::XM_PI - 0.1f);
    }
    else if (btnState & MK_RBUTTON)
    {
        // Make each pixel correspond to 0.005 unit in the scene.
        float dx = 0.005f * static_cast<float>(x - _LastMousePos.x);
        float dy = 0.005f * static_cast<float>(y - _LastMousePos.y);

        _Radius += dx - dy;

        _Radius = std::clamp(_Radius, 3.f, 15.f);
    }

    _LastMousePos.x = x;
    _LastMousePos.y = y;
}

void BoxApp::BuildDescriptorHeaps()
{
    D3D12_DESCRIPTOR_HEAP_DESC cbvHeapDesc{
        .Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV,
        .NumDescriptors = 1,
        .Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE,
        .NodeMask = 0};

    winrt::check_hresult(_Device->CreateDescriptorHeap(&cbvHeapDesc, IID_PPV_ARGS(_CBVHeap.put())));
}

void BoxApp::BuildConstantBuffers()
{
    _ObjectCB = std::make_unique<UploadBuffer<ObjectConstants>>(_Device.get(), 1, true);

    UINT objCByteSize = d3dUtil::CalcConstantBufferByteSize(sizeof(ObjectConstants));

    D3D12_GPU_VIRTUAL_ADDRESS cbAddress = _ObjectCB->Resource()->GetGPUVirtualAddress();

    // Offset to the ith object constant buffer in the buffer.
    int boxCBufIndex = 0;
    cbAddress += boxCBufIndex * objCByteSize;

    D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc{.BufferLocation = cbAddress, .SizeInBytes = objCByteSize};
    _Device->CreateConstantBufferView(&cbvDesc, _CBVHeap->GetCPUDescriptorHandleForHeapStart());
}

void BoxApp::BuildRootSignature()
{
    CD3DX12_ROOT_PARAMETER slotRootParameter[1];

    CD3DX12_DESCRIPTOR_RANGE cvbTable;
    cvbTable.Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0);
    slotRootParameter[0].InitAsDescriptorTable(1, &cvbTable);

    CD3DX12_ROOT_SIGNATURE_DESC rootSigDesc(
        1, slotRootParameter, 0, nullptr, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

    winrt::com_ptr<ID3DBlob> serializedRootSig;
    winrt::com_ptr<ID3DBlob> errorBlob;

    HRESULT hr = D3D12SerializeRootSignature(
        &rootSigDesc, D3D_ROOT_SIGNATURE_VERSION_1, serializedRootSig.put(), errorBlob.put());

    if (errorBlob)
    {
        ::OutputDebugStringA(static_cast<char*>(errorBlob->GetBufferPointer()));
    }

    winrt::check_hresult(hr);

    winrt::check_hresult(_Device->CreateRootSignature(
        0,
        serializedRootSig->GetBufferPointer(),
        serializedRootSig->GetBufferSize(),
        IID_PPV_ARGS(_RootSignature.put())));
}

void BoxApp::BuildShaderAndInputLayout()
{
    constexpr DWORD len = 1024;
    wchar_t exePath[len];
    GetModuleFileNameW(nullptr, exePath, len);
    std::filesystem::path p = exePath;
    std::wstring folderPath = p.parent_path().wstring();
    std::wstring path = folderPath + LR"(\Shaders\color.hlsl)";
    _VSByteCode = d3dUtil::CompileShader(path, nullptr, "VS", "vs_5_0");
    _PSByteCode = d3dUtil::CompileShader(path, nullptr, "PS", "ps_5_0");

    _InputLayout = {
        {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
        {"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0}};
}

void BoxApp::BuildBoxGeometry()
{
    constexpr size_t len = 100; 
    const float dx = 2.f / len;
    const float dz = DirectX::XM_2PI / len;

    std::array<Vertex, len / 2> vertices;
    std::array<std::uint16_t, len / 2> indices;
    for (size_t i = 0; i < len / 2; i++)
    {
        vertices[i]._Pos = XMFLOAT3(-1.f + dx * i, 0, std::sin(dz * i));
        vertices[i]._Color = XMFLOAT4(Colors::ForestGreen);
        indices[i] = (uint16_t)i;
    }

    const UINT verticeByteSize = static_cast<UINT>(vertices.size() * sizeof(Vertex));
    const UINT indexByteSize = static_cast<UINT>(indices.size() * sizeof(std::uint16_t));

    _BoxGeometry = std::make_unique<MeshGeometry>();
    _BoxGeometry->_Name = "boxGeometry";

    winrt::check_hresult(D3DCreateBlob(verticeByteSize, _BoxGeometry->_VertexBufferCPU.put()));
    CopyMemory(_BoxGeometry->_VertexBufferCPU->GetBufferPointer(), vertices.data(), verticeByteSize);

    winrt::check_hresult(D3DCreateBlob(indexByteSize, _BoxGeometry->_IndexBufferCPU.put()));
    CopyMemory(_BoxGeometry->_IndexBufferCPU->GetBufferPointer(), indices.data(), indexByteSize);

    _BoxGeometry->_VertexBufferGPU = d3dUtil::CreateDefaultBuffer(
        _Device, _CommandList, vertices.data(), verticeByteSize, _BoxGeometry->_VertexBufferUploader);

    _BoxGeometry->_IndexBufferGPU = d3dUtil::CreateDefaultBuffer(
        _Device, _CommandList, indices.data(), indexByteSize, _BoxGeometry->_IndexBufferUploader);

    _BoxGeometry->_VertexByteStride = sizeof(Vertex);
    _BoxGeometry->_VertexBufferByteSize = verticeByteSize;
    _BoxGeometry->_IndexFormat = DXGI_FORMAT_R16_UINT;
    _BoxGeometry->_IndexBufferByteSize = indexByteSize;

    SubmeshGeometry submesh;
    submesh._IndexCount = (UINT)indices.size();
    submesh._StartIndexLocation = 0;
    submesh._BaseVertexLocation = 0;

    _BoxGeometry->_DrawArgs["box"] = submesh;
}

void BoxApp::BuildPSO()
{
    D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc;
    ZeroMemory(&psoDesc, sizeof psoDesc);

    psoDesc.InputLayout = {_InputLayout.data(), (UINT)_InputLayout.size()};
    psoDesc.pRootSignature = _RootSignature.get();
    psoDesc.VS = {reinterpret_cast<BYTE*>(_VSByteCode->GetBufferPointer()), _VSByteCode->GetBufferSize()};
    psoDesc.PS = {reinterpret_cast<BYTE*>(_PSByteCode->GetBufferPointer()), _PSByteCode->GetBufferSize()};
    psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
    psoDesc.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME;
    psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
    psoDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
    psoDesc.SampleMask = UINT_MAX;
    psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE;
    psoDesc.NumRenderTargets = 1;
    psoDesc.RTVFormats[0] = _BackBufferFormat;
    psoDesc.SampleDesc.Count = _m4xMsaaState ? 4 : 1;
    psoDesc.SampleDesc.Quality = _m4xMsaaState ? (_m4xMsaaState - 1) : 0;
    psoDesc.DSVFormat = _DepthStencilFormat;

    winrt::check_hresult(_Device->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(_PSO.put())));
}

void BoxApp::UpdateGeometry(const ShapeViewer::Polyline& polyline)
{
    winrt::check_hresult(_CommandList->Reset(_DirectCmdListAlloc.get(), nullptr));

    const Polyline::Vertices points = polyline.Points();
    std::vector<Vertex> vertices;
    size_t len = points.size();
    vertices.resize(len);
    std::vector<std::uint16_t> indices;
    indices.resize(len);
    for (size_t i = 0; i < len; i++)
    {
        vertices[i] = points[i];
        indices[i] = (uint16_t)i;
    }

    const UINT verticeByteSize = static_cast<UINT>(vertices.size() * sizeof(Vertex));
    const UINT indexByteSize = static_cast<UINT>(indices.size() * sizeof(std::uint16_t));

    _BoxGeometry = std::make_unique<MeshGeometry>();
    _BoxGeometry->_Name = "boxGeometry";

    winrt::check_hresult(D3DCreateBlob(verticeByteSize, _BoxGeometry->_VertexBufferCPU.put()));
    CopyMemory(_BoxGeometry->_VertexBufferCPU->GetBufferPointer(), vertices.data(), verticeByteSize);

    winrt::check_hresult(D3DCreateBlob(indexByteSize, _BoxGeometry->_IndexBufferCPU.put()));
    CopyMemory(_BoxGeometry->_IndexBufferCPU->GetBufferPointer(), indices.data(), indexByteSize);

    _BoxGeometry->_VertexBufferGPU = d3dUtil::CreateDefaultBuffer(
        _Device, _CommandList, vertices.data(), verticeByteSize, _BoxGeometry->_VertexBufferUploader);

    _BoxGeometry->_IndexBufferGPU = d3dUtil::CreateDefaultBuffer(
        _Device, _CommandList, indices.data(), indexByteSize, _BoxGeometry->_IndexBufferUploader);

    _BoxGeometry->_VertexByteStride = sizeof(Vertex);
    _BoxGeometry->_VertexBufferByteSize = verticeByteSize;
    _BoxGeometry->_IndexFormat = DXGI_FORMAT_R16_UINT;
    _BoxGeometry->_IndexBufferByteSize = indexByteSize;

    SubmeshGeometry submesh;
    submesh._IndexCount = (UINT)indices.size();
    submesh._StartIndexLocation = 0;
    submesh._BaseVertexLocation = 0;

    _BoxGeometry->_DrawArgs["box"] = submesh;

    winrt::check_hresult(_CommandList->Close());
    ID3D12CommandList* cmdsList[] = {_CommandList.get()};
    _CommandQueue->ExecuteCommandLists(_countof(cmdsList), cmdsList);

    FlushCommandQueue();
}
