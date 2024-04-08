#pragma once

#include "d3dUtil.h"
#include "pch.h"

template <typename T> class UploadBuffer
{
  public:
    UploadBuffer(ID3D12Device* device, UINT elementCount, bool isConstantBuffer) : _IsConstantBuffer(isConstantBuffer)
    {
        _ElementByteSize = sizeof(T);

        if (isConstantBuffer)
        {
            _ElementByteSize = d3dUtil::CalcConstantBufferByteSize(_ElementByteSize);
        }

        CD3DX12_HEAP_PROPERTIES uploadProperties(D3D12_HEAP_TYPE_UPLOAD);
        auto buffer = CD3DX12_RESOURCE_DESC::Buffer(_ElementByteSize * elementCount);
        winrt::check_hresult(device->CreateCommittedResource(
            &uploadProperties,
            D3D12_HEAP_FLAG_NONE,
            &buffer,
            D3D12_RESOURCE_STATE_GENERIC_READ,
            nullptr,
            IID_PPV_ARGS(&_UploadBuffer)));

        winrt::check_hresult(_UploadBuffer->Map(0, nullptr, reinterpret_cast<void**>(&_MappedData)));

        // We do not need to unmap until we are done with the resource.
        // However, we must not write to the resource while it is in use by the GPU ( so we must
        // use synchronization techniques).
    }

    UploadBuffer(const UploadBuffer&) = delete;
    UploadBuffer& operator=(const UploadBuffer&) = delete;

    ~UploadBuffer()
    {
        if (_UploadBuffer)
        {
            _UploadBuffer->Unmap(0, nullptr);
        }

        _MappedData = nullptr;
    }

    winrt::com_ptr<ID3D12Resource> Resource() const
    {
        return _UploadBuffer;
    }

    void CopyData(int elementIndex, const T& data)
    {
        memcpy(&_MappedData[elementIndex * _ElementByteSize], &data, sizeof(T));
    }

  private:
    bool _IsConstantBuffer = false;
    UINT _ElementByteSize = 0;
    winrt::com_ptr<ID3D12Resource> _UploadBuffer;
    BYTE* _MappedData = nullptr;
};
