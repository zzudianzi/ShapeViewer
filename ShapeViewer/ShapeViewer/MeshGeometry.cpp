#include "pch.h"
#include "MeshGeometry.h"

D3D12_VERTEX_BUFFER_VIEW MeshGeometry::VPosBufferView() const
{
    return {
        .BufferLocation = _VPosBufferGPU->GetGPUVirtualAddress(),
        .SizeInBytes = _VPosBufferByteSize,
        .StrideInBytes = _VPosByteStride};
}

D3D12_VERTEX_BUFFER_VIEW MeshGeometry::VColorBufferView() const
{
    return {
        .BufferLocation = _VColorBufferGPU->GetGPUVirtualAddress(),
        .SizeInBytes = _VColorBufferByteSize,
        .StrideInBytes = _VColorByteStride};
}

D3D12_INDEX_BUFFER_VIEW MeshGeometry::IndexBufferView() const
{
    return {
        .BufferLocation = _IndexBufferGPU->GetGPUVirtualAddress(),
        .SizeInBytes = _IndexBufferByteSize,
        .Format = _IndexFormat};
}

void MeshGeometry::DisposeUploaders()
{
    _VPosBufferUploader = nullptr;
    _VColorBufferUploader = nullptr;
    _IndexBufferUploader = nullptr;
}
