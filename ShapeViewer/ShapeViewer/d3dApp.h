#pragma once

#if defined(DEBUG) || defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif

// Link necessary d3d12 libraries.
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "D3D12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")

#include <d3d12.h>
#include <dxgi1_5.h>
#include <winrt/base.h>
#include <microsoft.ui.xaml.media.dxinterop.h>

#include "GameTimer.h"

#include "Polyline.h"

#include "D2DWrapper.h"

namespace ShapeViewer
{
class D3DApp
{
  public:
    D3DApp(winrt::com_ptr<ISwapChainPanelNative> panelNative);
    virtual ~D3DApp();

    virtual bool InitDirect3D();

    virtual void OnResize(float width, float height);
    virtual void Update() = 0;
    virtual void Draw() = 0;

    virtual void Draw2D();

    virtual void OnMouseDown(WPARAM btnState, int x, int y);
    virtual void OnMouseUp(WPARAM btnState, int x, int y);
    virtual void OnMouseMove(WPARAM btnState, int x, int y);
    virtual void OnMouseWheel(WPARAM btnState, int x, int y, int delta);

    float AspectRatio() const;

    virtual void UpdateGeometry(const ::ShapeViewer::Polyline& polyline)
    {
    }

    const Display& GetDisplay() const;
    Display& GetDisplay();

  protected:
    void CreateCommandObjects();
    void CreateSwapChain();
    void FlushCommandQueue();

    virtual void CreateRtvAndDsvDescriptorHeaps();

    ID3D12Resource* CurrentBackBuffer() const;

    D3D12_CPU_DESCRIPTOR_HANDLE CurrentBackBufferView() const;
    D3D12_CPU_DESCRIPTOR_HANDLE DepthStencilView() const;

    void AssociateSwapChain();

  protected:
    winrt::com_ptr<ISwapChainPanelNative> _PanelNative;

    winrt::com_ptr<IDXGIFactory5> _Factory;
    winrt::com_ptr<ID3D12Device> _Device;

    winrt::com_ptr<IDXGISwapChain3> _SwapChain;
    static constexpr int _SwapChainBufferCount = 2;
    int _CurrentBackBuffer = 0;
    winrt::com_ptr<ID3D12Resource> _SwapChainBuffer[_SwapChainBufferCount];
    DXGI_FORMAT _BackBufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM;

    UINT _RTVDescriptorSize = 0;
    UINT _DSVDescriptorSize = 0;
    UINT _CBVSrvDescriptorSize = 0;
    winrt::com_ptr<ID3D12DescriptorHeap> _RTVHeap;
    winrt::com_ptr<ID3D12DescriptorHeap> _DSVHeap;

    bool _m4xMsaaState = false; // 4X MSAA enabled
    UINT _m4xMsaaQuality = 0;   // quality level of 4X MSAA

    winrt::com_ptr<ID3D12CommandQueue> _CommandQueue;
    winrt::com_ptr<ID3D12CommandAllocator> _DirectCmdListAlloc;
    winrt::com_ptr<ID3D12GraphicsCommandList> _CommandList;

    winrt::com_ptr<ID3D12Fence> _Fence;
    UINT64 _CurrentFence = 0;

    winrt::com_ptr<ID3D12Resource> _DepthStencilBuffer;
    DXGI_FORMAT _DepthStencilFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

    int _ClientWidth = 512;
    int _ClientHeight = 512;

    D3D12_VIEWPORT _ScreenViewport;
    D3D12_RECT _ScissorRect;

    D2DWrapper _D2DWrapper;
};

} // namespace ShapeViewer
