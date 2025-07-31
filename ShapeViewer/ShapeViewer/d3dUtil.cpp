#include "pch.h"
#include "d3dUtil.h"

#include "d3dx12.h"
#include <Shlwapi.h>

#include <fstream>

#pragma comment(lib, "Shlwapi.lib")

winrt::com_ptr<ID3D12Resource> d3dUtil::CreateDefaultBuffer(
    winrt::com_ptr<ID3D12Device>& device,
    winrt::com_ptr<ID3D12GraphicsCommandList> commandList,
    const void* initData,
    UINT64 byteSize,
    winrt::com_ptr<ID3D12Resource>& uploadBuffer)
{
    winrt::com_ptr<ID3D12Resource> defaultBuffer;

    auto heapProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
    auto bufferDesc = CD3DX12_RESOURCE_DESC::Buffer(byteSize);
    winrt::check_hresult(device->CreateCommittedResource(
        &heapProperties,
        D3D12_HEAP_FLAG_NONE,
        &bufferDesc,
        D3D12_RESOURCE_STATE_COMMON,
        nullptr,
        IID_PPV_ARGS(defaultBuffer.put())));

    // In order to copy CPU memory data into our default buffer, we need to
    // create an intermediate upload heap

    auto uploadBufferHeapProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
    auto uploadBufferDesc = CD3DX12_RESOURCE_DESC::Buffer(byteSize);
    winrt::check_hresult(device->CreateCommittedResource(
        &uploadBufferHeapProperties,
        D3D12_HEAP_FLAG_NONE,
        &uploadBufferDesc,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(uploadBuffer.put())));

    // Describe the data we want to copy into the default buffer.
    D3D12_SUBRESOURCE_DATA subResourceData{
        .pData = initData, .RowPitch = static_cast<LONG_PTR>(byteSize), .SlicePitch = static_cast<LONG_PTR>(byteSize)};

    // Schedule to copy the data to the default buffer resource.
    // At a high level, the helper function UpdateSubresources will copy the CPU memory into the
    // intermediate upload heap.
    // Then using ID3D12CommandList::CopySubresourceRegion the intermediate upload heap data will
    // be copied to _Buffer
    auto commonToCopyTransition = CD3DX12_RESOURCE_BARRIER::Transition(
        defaultBuffer.get(), D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_COPY_DEST);
    commandList->ResourceBarrier(1, &commonToCopyTransition);
    UpdateSubresources<1>(commandList.get(), defaultBuffer.get(), uploadBuffer.get(), 0, 0, 1, &subResourceData);
    auto copyToReadTransition = CD3DX12_RESOURCE_BARRIER::Transition(
        defaultBuffer.get(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_GENERIC_READ);
    commandList->ResourceBarrier(1, &copyToReadTransition);

    // Note: uploadBuffer has to be kept alive after the above function calls because the command list
    // has not been executed yet that performs the actual copy.
    // The caller can Release the uploadBuffer after it knows the copy has been executed.

    return defaultBuffer;
}

UINT d3dUtil::CalcConstantBufferByteSize(UINT byteSize)
{
    // Constant buffers must be a multiple of the minimum hardware allocation size (usually 256 bytes)
    // So round up to nearest multiple of 256. We do this by adding 255 and then masking off the
    // lower 2 bytes which store all bits < 256

    return (byteSize + 255) & ~255;
}

winrt::com_ptr<ID3DBlob> d3dUtil::CompileShader(
    const std::wstring& filename,
    const D3D_SHADER_MACRO* defines,
    const std::string& entryPoint,
    const std::string& target)
{
    // Use debug flags in debug mode.
    UINT compileFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)
    compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

    HRESULT hr = S_OK;

    winrt::com_ptr<ID3DBlob> byteCode;
    winrt::com_ptr<ID3DBlob> errors;

    hr = D3DCompileFromFile(
        filename.c_str(),
        defines,
        D3D_COMPILE_STANDARD_FILE_INCLUDE,
        entryPoint.c_str(),
        target.c_str(),
        compileFlags,
        0,
        byteCode.put(),
        errors.put());

    // Output errors to debug window.
    if (errors)
    {
        OutputDebugStringA((char*)errors->GetBufferPointer());

        winrt::check_hresult(hr);
    }

    winrt::check_hresult(hr);

    return byteCode;
}

winrt::com_ptr<ID3DBlob> d3dUtil::LoadBinary(const std::wstring& filename)
{
    std::ifstream fin(filename, std::ios::binary);

    fin.seekg(0, std::ios_base::end);
    std::ifstream::pos_type size = static_cast<int>(fin.tellg());
    fin.seekg(0, std::ios_base::beg);

    winrt::com_ptr<ID3DBlob> blob;
    winrt::check_hresult(D3DCreateBlob(size, blob.put()));

    fin.read(static_cast<char*>(blob->GetBufferPointer()), size);
    fin.close();

    return blob;
}
