#include "pch.h"
#include "D2DWrapper.h"

using namespace ShapeViewer;

D2DWrapper::D2DWrapper(int swapChainBufferCount)
{
    _WrappedBackBuffers.resize(swapChainBufferCount);
    _Bitmaps.resize(swapChainBufferCount);
}

bool D2DWrapper::InitDirect2D(winrt::com_ptr<ID3D12Device> device, winrt::com_ptr<ID3D12CommandQueue> commandQueue)
{
    UINT d3d11DeviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
    D2D1_FACTORY_OPTIONS d2dFactoryOptions = {};

#if defined(_DEBUG) || defined(DEBUG)
    d2dFactoryOptions.debugLevel = D2D1_DEBUG_LEVEL_INFORMATION;

    d3d11DeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    D3D_FEATURE_LEVEL level;

    winrt::com_ptr<ID3D11Device> d11Device;

    auto s = commandQueue.get();
    winrt::check_hresult(D3D11On12CreateDevice(
        device.get(),
        d3d11DeviceFlags,
        nullptr,
        0,
        reinterpret_cast<IUnknown**>(&s),
        1,
        0,
        d11Device.put(),
        _D11Context.put(),
        &level));

    _D11Device = d11Device.try_as<ID3D11On12Device>();

    // Create D2D/DWrite components
    {
        D2D1_DEVICE_CONTEXT_OPTIONS deviceOptions = D2D1_DEVICE_CONTEXT_OPTIONS_NONE;

        winrt::check_hresult(D2D1CreateFactory(
            D2D1_FACTORY_TYPE_SINGLE_THREADED, __uuidof(ID2D1Factory3), &d2dFactoryOptions, _D2D1Factory.put_void()));

        winrt::com_ptr<IDXGIDevice> dxgiDevice;
        dxgiDevice = _D11Device.as<IDXGIDevice>();

        winrt::check_hresult(_D2D1Factory->CreateDevice(dxgiDevice.get(), _D2D1Device.put()));

        winrt::check_hresult(_D2D1Device->CreateDeviceContext(deviceOptions, _D2D1Context.put()));

        winrt::com_ptr<IUnknown> dwriteFactory;
        winrt::check_hresult(
            DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), dwriteFactory.put()));

        _DWriteFactory = dwriteFactory.as<IDWriteFactory>();
    }

    // Create D2D/Dwrite objects for rendering text
    {
        winrt::check_hresult(_D2D1Context->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), _Brush.put()));

        winrt::check_hresult(_DWriteFactory->CreateTextFormat(
            L"Verdana",
            nullptr,
            DWRITE_FONT_WEIGHT_NORMAL,
            DWRITE_FONT_STYLE_NORMAL,
            DWRITE_FONT_STRETCH_NORMAL,
            50,
            L"en-us",
            _TextFormat.put()));

        winrt::check_hresult(_TextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER));
        winrt::check_hresult(_TextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER));
    }

    _Display.D2D1Factory() = _D2D1Factory;
    _Display.RenderTarget() = _D2D1Context;
    _Display.Brush() = _Brush;

    return true;
}

void D2DWrapper::ReleaseResource()
{
    // Release the previous resource we will be recreating.
    for (int i = 0; i < (int)_WrappedBackBuffers.size(); i++)
    {
        _WrappedBackBuffers[i] = nullptr;
        _Bitmaps[i] = nullptr;
    }

    _D2D1Context->SetTarget(nullptr);
    _D11Context->Flush();
}

void D2DWrapper::CreateWrappedResource(winrt::com_ptr<ID3D12Resource> swapChainBuffer, int bufferIndex)
{
    D3D11_RESOURCE_FLAGS d3d11Flags = {D3D11_BIND_RENDER_TARGET};
    winrt::check_hresult(_D11Device->CreateWrappedResource(
        swapChainBuffer.get(),
        &d3d11Flags,
        D3D12_RESOURCE_STATE_RENDER_TARGET,
        D3D12_RESOURCE_STATE_PRESENT,
        IID_PPV_ARGS(_WrappedBackBuffers[bufferIndex].put())));

    winrt::com_ptr<IDXGISurface> surface;
    surface = _WrappedBackBuffers[bufferIndex].as<IDXGISurface>();

    float dpi = 96.f;
    D2D1_BITMAP_PROPERTIES1 bitmapProperties = D2D1::BitmapProperties1(
        D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
        D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED),
        dpi,
        dpi);
    winrt::check_hresult(
        _D2D1Context->CreateBitmapFromDxgiSurface(surface.get(), &bitmapProperties, _Bitmaps[bufferIndex].put()));
}

void D2DWrapper::Draw(int bufferIndex)
{
    D2D1_SIZE_F rtSize = _Bitmaps[bufferIndex]->GetSize();
    D2D1_RECT_F textRect = D2D1::RectF(0, 0, rtSize.width, rtSize.height);
    static const WCHAR text[] = L"11On12";

    // Acquire our wrapped render target resource for the current back buffer.
    auto s = _WrappedBackBuffers[bufferIndex].get();

    _D11Device->AcquireWrappedResources(&s, 1);

    // Render text directly to the back buffer.
    _D2D1Context->SetTarget(_Bitmaps[bufferIndex].get());
    _D2D1Context->BeginDraw();
    _D2D1Context->SetTransform(D2D1::Matrix3x2F::Identity());
    _Display.Draw();
    winrt::check_hresult(_D2D1Context->EndDraw());

    // Release our wrapped render target resource. Releasing
    // transitions the back buffer resource to the state specified
    // as the OutState when the wrapped resource was created.
    _D11Device->ReleaseWrappedResources(&s, 1);

    // Flush to submit the 11 command list to the shared command queue.
    _D11Context->Flush();
}

const Display& D2DWrapper::GetDisplay() const
{
    return _Display;
}

Display& D2DWrapper::GetDisplay()
{
    return _Display;
}