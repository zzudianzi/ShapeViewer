#pragma once

#include <d3d11on12.h>
#include <d2d1.h>
#include <d2d1_3.h>
#include <dwrite.h>
#include <winrt/base.h>

#include "Display.h"

namespace ShapeViewer
{
class D2DWrapper
{
  public:
    D2DWrapper(int swapChainBufferCount);

    bool InitDirect2D(winrt::com_ptr<ID3D12Device> device, winrt::com_ptr<ID3D12CommandQueue> commandQueue);

    void ReleaseResource();
    void CreateWrappedResource(winrt::com_ptr<ID3D12Resource> swapChainBuffer, int bufferIndex);

    void Draw(int bufferIndex);

    const Display& GetDisplay() const;
    Display& GetDisplay();

  private:
    // d3d 11 on 12
    winrt::com_ptr<ID3D11On12Device> _D11Device;
    winrt::com_ptr<ID3D11DeviceContext> _D11Context;

    // d2d
    winrt::com_ptr<ID2D1Device2> _D2D1Device;
    winrt::com_ptr<ID2D1DeviceContext2> _D2D1Context;
    winrt::com_ptr<ID2D1Factory3> _D2D1Factory;
    winrt::com_ptr<IDWriteFactory> _DWriteFactory;
    std::vector<winrt::com_ptr<ID3D11Resource>> _WrappedBackBuffers;
    std::vector<winrt::com_ptr<ID2D1Bitmap1>> _Bitmaps;
    winrt::com_ptr<ID2D1SolidColorBrush> _Brush;
    winrt::com_ptr<IDWriteTextFormat> _TextFormat;

    Display _Display;
};
} // namespace ShapeViewer
