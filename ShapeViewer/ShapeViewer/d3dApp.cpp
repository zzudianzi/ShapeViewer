#include "pch.h"
#include "d3dApp.h"

#include "d3dx12.h"

D3DApp::D3DApp(winrt::com_ptr<ISwapChainPanelNative> panelNative)
{
    _PanelNative = panelNative;
}

D3DApp::~D3DApp()
{
    if (_Device != nullptr)
    {
        FlushCommandQueue();
    }
}

bool D3DApp::InitDirect3D()
{
#if defined(DEBUG) || defined(_DEBUG)
    // Enable the D3D12 debug layer.
    {
        winrt::com_ptr<ID3D12Debug> debugController;
        winrt::check_hresult(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)));
        debugController->EnableDebugLayer();
    }
#endif

    winrt::check_hresult(CreateDXGIFactory1(IID_PPV_ARGS(&_Factory)));

    // Create d3d device
    HRESULT hardwareResult = D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&_Device));

    // Fallback to WARP device
    if (FAILED(hardwareResult))
    {
        winrt::com_ptr<IDXGIAdapter> warpAdapter;
        winrt::check_hresult(_Factory->EnumWarpAdapter(IID_PPV_ARGS(&warpAdapter)));
        winrt::check_hresult(D3D12CreateDevice(warpAdapter.get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&_Device)));
    }

    // Create the Fence
    winrt::check_hresult(_Device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(_Fence.put())));

    // Create the Fence and Descriptor Sizes
    winrt::com_ptr<ID3D12Fence> fence;
    winrt::check_hresult(_Device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence)));

    _RTVDescriptorSize = _Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
    _DSVDescriptorSize = _Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
    _CBVSrvDescriptorSize = _Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

    // Check 4X MSAA Quality Support
    DXGI_FORMAT backBufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
    D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS qualityLevels;
    qualityLevels.Format = backBufferFormat;
    qualityLevels.SampleCount = 4;
    qualityLevels.Flags = D3D12_MULTISAMPLE_QUALITY_LEVELS_FLAG_NONE;
    qualityLevels.NumQualityLevels = 0;

    winrt::check_hresult(
        _Device->CheckFeatureSupport(D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS, &qualityLevels, sizeof qualityLevels));
    _m4xMsaaQuality = qualityLevels.NumQualityLevels;
    assert(_m4xMsaaQuality > 0 && "Unexpected MSAA quality level.");

    CreateCommandObjects();
    CreateSwapChain();
    CreateRtvAndDsvDescriptorHeaps();

    return true;
}

void D3DApp::CreateCommandObjects()
{
    // Create command queue and command list
    D3D12_COMMAND_QUEUE_DESC queueDesc = {
        .Type = D3D12_COMMAND_LIST_TYPE_DIRECT, .Flags = D3D12_COMMAND_QUEUE_FLAG_NONE};

    winrt::check_hresult(_Device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(_CommandQueue.put())));
    winrt::check_hresult(
        _Device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(_DirectCmdListAlloc.put())));
    winrt::check_hresult(_Device->CreateCommandList(
        0, D3D12_COMMAND_LIST_TYPE_DIRECT, _DirectCmdListAlloc.get(), nullptr, IID_PPV_ARGS(_CommandList.put())));

    _CommandList->Close();
}

void D3DApp::CreateSwapChain()
{
    // Create swap chain
    DXGI_SWAP_CHAIN_DESC1 swapChainDesc{0};
    swapChainDesc.Width = _ClientWidth;
    swapChainDesc.Height = _ClientHeight;
    swapChainDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    swapChainDesc.Stereo = false;
    swapChainDesc.SampleDesc.Count = _m4xMsaaState ? 4 : 1;
    swapChainDesc.SampleDesc.Quality = _m4xMsaaState ? (_m4xMsaaQuality - 1) : 0;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.BufferCount = 2;
    swapChainDesc.Scaling = DXGI_SCALING_STRETCH;
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
    swapChainDesc.Flags = 0;

    winrt::check_hresult(
        _Factory->CreateSwapChainForComposition(_CommandQueue.get(), &swapChainDesc, nullptr, _SwapChain.put()));
}

void D3DApp::AssociateSwapChain()
{
    // Associate to swap chain panel
    winrt::check_hresult(_PanelNative->SetSwapChain(_SwapChain.get()));
}

void D3DApp::FlushCommandQueue()
{
    _CurrentFence++;

    winrt::check_hresult(_CommandQueue->Signal(_Fence.get(), _CurrentFence));

    // Wait until the GPU has completed commands up to this fence point.
    if (_Fence->GetCompletedValue() < _CurrentFence)
    {
        HANDLE eventHandle = CreateEventEx(nullptr, nullptr, 0, EVENT_ALL_ACCESS);

        winrt::check_hresult(_Fence->SetEventOnCompletion(_CurrentFence, eventHandle));

        WaitForSingleObject(eventHandle, INFINITE);
        CloseHandle(eventHandle);
    }
}

void D3DApp::CreateRtvAndDsvDescriptorHeaps()
{
    // Create descriptor heap
    D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc{
        .Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV,
        .NumDescriptors = _SwapChainBufferCount,
        .Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE,
        .NodeMask = 0};
    winrt::check_hresult(_Device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(_RTVHeap.put())));

    D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc{
        .Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV,
        .NumDescriptors = 1,
        .Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE,
        .NodeMask = 0};

    winrt::check_hresult(_Device->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(_DSVHeap.put())));
}

void D3DApp::OnResize(float width, float height)
{
    assert(_Device);
    assert(_SwapChain);
    assert(_CommandList);
    assert(_DirectCmdListAlloc);

    _ClientWidth = (int)width;
    _ClientHeight = (int)height;

    FlushCommandQueue();

    _CommandList->Reset(_DirectCmdListAlloc.get(), nullptr);

    // Release the previous resource we will be recreating.
    for (int i = 0; i < _SwapChainBufferCount; i++)
    {
        _SwapChainBuffer[i] = nullptr;
    }

    _DepthStencilBuffer = nullptr;

    // Resize the swap chain
    winrt::check_hresult(_SwapChain->ResizeBuffers(
        _SwapChainBufferCount, _ClientWidth, _ClientHeight, _BackBufferFormat, DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH));
    AssociateSwapChain();

    _CurrentBackBuffer = 0;

    // Create target view for each buffer
    CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHeapHandle(_RTVHeap->GetCPUDescriptorHandleForHeapStart());
    for (UINT i = 0; i < _SwapChainBufferCount; i++)
    {
        winrt::check_hresult(_SwapChain->GetBuffer(i, IID_PPV_ARGS(_SwapChainBuffer[i].put())));
        _Device->CreateRenderTargetView(_SwapChainBuffer[i].get(), nullptr, rtvHeapHandle);
        rtvHeapHandle.Offset(1, _RTVDescriptorSize);
    }

    // Create the depth/stencil buffer and view
    D3D12_RESOURCE_DESC depthStencilDesc{
        .Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D,
        .Alignment = 0,
        .Width = static_cast<UINT64>(_ClientWidth),
        .Height = static_cast<UINT64>(_ClientHeight),
        .DepthOrArraySize = 1,
        .MipLevels = 1,
        .Format = DXGI_FORMAT_R24G8_TYPELESS,
        .SampleDesc{
            .Count = _m4xMsaaState ? 4u : 1u,
            .Quality = _m4xMsaaState ? (_m4xMsaaQuality - 1) : 0,
        },
        .Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN,
        .Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL};

    D3D12_CLEAR_VALUE optClear{.Format = _DepthStencilFormat, .DepthStencil{.Depth = 1.f, .Stencil = 0}};

    CD3DX12_HEAP_PROPERTIES d3dHeapProperties(D3D12_HEAP_TYPE_DEFAULT);
    winrt::check_hresult(_Device->CreateCommittedResource(
        &d3dHeapProperties,
        D3D12_HEAP_FLAG_NONE,
        &depthStencilDesc,
        D3D12_RESOURCE_STATE_COMMON,
        &optClear,
        IID_PPV_ARGS(_DepthStencilBuffer.put())));

    // Create descriptor to mip level 0 of entire resource using the format of the resource
    D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc{
        .Format = _DepthStencilFormat,
        .ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D,
        .Flags = D3D12_DSV_FLAG_NONE,
        .Texture2D{.MipSlice = 0}};
    _Device->CreateDepthStencilView(_DepthStencilBuffer.get(), &dsvDesc, DepthStencilView());

    // Transition the resource from its initial state to be used as a depth buffer
    auto d3dTransition = CD3DX12_RESOURCE_BARRIER::Transition(
        _DepthStencilBuffer.get(), D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_DEPTH_WRITE);
    _CommandList->ResourceBarrier(1, &d3dTransition);

    // Execute the resize commands
    winrt::check_hresult(_CommandList->Close());
    ID3D12CommandList* cmdsLists[] = {_CommandList.get()};
    _CommandQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);

    // Wait until resize is complete
    FlushCommandQueue();

    // Update the viewport transform to cover the client area.
    _ScreenViewport.TopLeftX = 0;
    _ScreenViewport.TopLeftY = 0;
    _ScreenViewport.Width = static_cast<float>(_ClientWidth);
    _ScreenViewport.Height = static_cast<float>(_ClientHeight);
    _ScreenViewport.MinDepth = 0.f;
    _ScreenViewport.MaxDepth = 1.f;

    _ScissorRect = {0, 0, _ClientWidth, _ClientHeight};

    _CommandList->RSSetScissorRects(1, &_ScissorRect);
}

ID3D12Resource* D3DApp::CurrentBackBuffer() const
{
    return _SwapChainBuffer[_CurrentBackBuffer].get();
}

D3D12_CPU_DESCRIPTOR_HANDLE D3DApp::CurrentBackBufferView() const
{
    return CD3DX12_CPU_DESCRIPTOR_HANDLE(
        _RTVHeap->GetCPUDescriptorHandleForHeapStart(), _CurrentBackBuffer, _RTVDescriptorSize);
}

D3D12_CPU_DESCRIPTOR_HANDLE D3DApp::DepthStencilView() const
{
    return _DSVHeap->GetCPUDescriptorHandleForHeapStart();
}

float D3DApp::AspectRatio() const
{
    return static_cast<float>(_ClientWidth) / _ClientHeight;
}

void D3DApp::OnMouseDown(WPARAM btnState, int x, int y)
{

}

void D3DApp::OnMouseUp(WPARAM btnState, int x, int y)
{

}

void D3DApp::OnMouseMove(WPARAM btnState, int x, int y)
{

}
