#include "pch.h"
#include "MeshGeometry.h"

D3D12_VERTEX_BUFFER_VIEW MeshGeometry::VertexBufferView() const
{
    return {
        .BufferLocation = _VertexBufferGPU->GetGPUVirtualAddress(),
        .SizeInBytes = _VertexBufferByteSize,
        .StrideInBytes = _VertexByteStride};
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
    _VertexBufferUploader = nullptr;
    _IndexBufferUploader = nullptr;
}
