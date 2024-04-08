#pragma once

#include <Windows.h>
#include <winrt/base.h>

#include <dxgi1_4.h>
#include <d3dcompiler.h>

#include "MeshGeometry.h"

struct ID3D12Resource;
struct ID3D12Device;
struct ID3D12GraphicsCommandList;

class d3dUtil
{
  public:
    static winrt::com_ptr<ID3D12Resource> CreateDefaultBuffer(
        winrt::com_ptr<ID3D12Device>& device,
        winrt::com_ptr<ID3D12GraphicsCommandList> commandList,
        const void* initData,
        UINT64 byteSize,
        winrt::com_ptr<ID3D12Resource>& uploadBuffer);

    static UINT CalcConstantBufferByteSize(UINT byteSize);

    static winrt::com_ptr<ID3DBlob> CompileShader(
        const std::wstring& filename,
        const D3D_SHADER_MACRO* defines,
        const std::string& entryPoint,
        const std::string& target);
};
