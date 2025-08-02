#pragma once

#include <string>
#include <unordered_map>

#include <d3dcompiler.h>
#include <winrt/base.h>
#include <DirectXCollision.h>
#include "d3dx12.h"

class SubmeshGeometry
{
  public:
    UINT _IndexCount = 0;
    UINT _StartIndexLocation = 0;
    INT _BaseVertexLocation = 0;

    DirectX::BoundingBox _Bounds;
};

class MeshGeometry
{
  public:
    std::string _Name;

    winrt::com_ptr<ID3DBlob> _VPosBufferCPU;
    winrt::com_ptr<ID3DBlob> _VColorBufferCPU;
    winrt::com_ptr<ID3DBlob> _IndexBufferCPU;

    winrt::com_ptr<ID3D12Resource> _VPosBufferGPU;
    winrt::com_ptr<ID3D12Resource> _VColorBufferGPU;
    winrt::com_ptr<ID3D12Resource> _IndexBufferGPU;

    winrt::com_ptr<ID3D12Resource> _VPosBufferUploader;
    winrt::com_ptr<ID3D12Resource> _VColorBufferUploader;
    winrt::com_ptr<ID3D12Resource> _IndexBufferUploader;

    UINT _VPosByteStride = 0;
    UINT _VPosBufferByteSize = 0;
    UINT _VColorByteStride = 0;
    UINT _VColorBufferByteSize = 0;
    DXGI_FORMAT _IndexFormat = DXGI_FORMAT_R16_UINT;
    UINT _IndexBufferByteSize = 0;

    // A Mesh geometry may store multiple geometries in one vertex/index buffer.
    // Use this container to define the Sub-mesh geometries so we can draw the
    // Sub-meshes individually.
    std::unordered_map<std::string, SubmeshGeometry> _DrawArgs;

    D3D12_VERTEX_BUFFER_VIEW VPosBufferView() const;
    D3D12_VERTEX_BUFFER_VIEW VColorBufferView() const;
    D3D12_INDEX_BUFFER_VIEW IndexBufferView() const;
    void DisposeUploaders();
};
