#include "pch.h"
#include "MainWindow.xaml.h"
#if __has_include("MainWindow.g.cpp")
#include "MainWindow.g.cpp"
#endif

#include "BoxApp.h"
#include <winrt/Microsoft.UI.Input.h>
#include <winrt/Windows.Storage.Pickers.h>
#include <winrt/Windows.UI.Xaml.Interop.h>
#include <microsoft.ui.xaml.window.h>
#include <ShObjIdl_core.h>
#include "MainWindowViewModel.h"
#include <JsonHelper.h>

#include "ROIPolygon.h"
#include "Display.h"
#include "ROIPolyline.h"
#include "ROIRect.h"
#include "Overlay.h"
#include "VisText.h"

using namespace winrt;
using namespace Windows::Foundation;
using namespace Microsoft::UI::Input;
using namespace Microsoft::UI::Xaml;
using namespace Microsoft::UI::Xaml::Input;
using namespace Microsoft::UI::Xaml::Controls;
using namespace Microsoft::UI::Xaml::Data;
using namespace Microsoft::UI::Xaml::Media;
using namespace Microsoft::UI::Composition;
using namespace Windows::Storage;
using namespace winrt::ShapeViewer::implementation;

MainWindow::MainWindow()
{
    _ViewModel = winrt::make<MainWindowViewModel>();
}

MainWindow::~MainWindow() = default;

void MainWindow::InitializeComponent()
{
    MainWindowT::InitializeComponent();

    _Timer.Reset();
}

winrt::ShapeViewer::MainWindowViewModel MainWindow::ViewModel()
{
    return _ViewModel;
}

void MainWindow::CalculateFrameStats()
{
    static int frameCount = 0;
    static double timeEplapsed = 0.;

    frameCount++;

    if ((_Timer.TotalTimeInSeconds() - timeEplapsed) >= 1.)
    {
        double fps = frameCount;
        double mspf = 1000. / fps;

        std::wstring fpsStr = std::to_wstring(fps);
        std::wstring mspfStr = std::to_wstring(mspf);

        std::wstring windowText = L"ShapeViwer....fps: " + fpsStr + L"   mspf: " + mspfStr;

        //Title(windowText.c_str());

        frameCount = 0;
        timeEplapsed += 1.;
    }
}

void MainWindow::swapChainPanel_Loaded(IInspectable const& sender, RoutedEventArgs const& e)
{
    _d3dApp.reset(new ::ShapeViewer::BoxApp(swapChainPanel().as<ISwapChainPanelNative>()));
    _d3dApp->InitDirect3D();
    _d3dApp->OnResize(swapChainPanel().ActualWidth(), swapChainPanel().ActualHeight());

    auto& display = _d3dApp->GetDisplay();
    auto overlay = display.CreateOverlay(false);

    _Text = new ::ShapeViewer::VisText(L"");
    overlay->AddItem(_Text);

    _RenderingToken = CompositionTarget::Rendering({this, &MainWindow::swapChainPanel_RenderingHandler});
}

void MainWindow::swapChainPanel_Unloaded(IInspectable const& sender, RoutedEventArgs const& e)
{
    _d3dApp.reset(nullptr);
    CompositionTarget::Rendering(_RenderingToken);
}

void MainWindow::swapChainPanel_SizeChanged(IInspectable const& sender, SizeChangedEventArgs const& e)
{
    if (swapChainPanel().IsLoaded())
    {
        Size size = e.NewSize();
        _d3dApp->OnResize(size.Width, size.Height);
    }
}

void MainWindow::swapChainPanel_RenderingHandler(IInspectable const& sender, IInspectable const& args)
{
    if (_WindowClosed || !_d3dApp)
    {
        return;
    }
    CalculateFrameStats();
    _d3dApp->Update();
    _d3dApp->Draw();
}

void MainWindow::swapChainPanel_PointerPressed(IInspectable const& sender, PointerRoutedEventArgs const& e)
{
    if (e.Pointer().PointerDeviceType() != PointerDeviceType::Mouse)
    {
        return;
    }

    swapChainPanel().CapturePointer(e.Pointer());

    WPARAM btnState = 0;
    auto pointerPoint = e.GetCurrentPoint(swapChainPanel());
    if (pointerPoint.Properties().IsLeftButtonPressed())
    {
        btnState |= MK_LBUTTON;
    }

    if (pointerPoint.Properties().IsRightButtonPressed())
    {
        btnState |= MK_RBUTTON;
    }
    auto position = pointerPoint.Position();

    _d3dApp->OnMouseDown(btnState, position.X, position.Y);
}

void MainWindow::swapChainPanel_PointerReleased(IInspectable const& sender, PointerRoutedEventArgs const& e)
{
    WPARAM btnState = 0;
    auto pointerPoint = e.GetCurrentPoint(swapChainPanel());
    if (pointerPoint.Properties().IsLeftButtonPressed())
    {
        btnState |= MK_LBUTTON;
    }

    if (pointerPoint.Properties().IsRightButtonPressed())
    {
        btnState |= MK_RBUTTON;
    }
    auto position = pointerPoint.Position();
    _d3dApp->OnMouseUp(btnState, position.X, position.Y);
    swapChainPanel().ReleasePointerCapture(e.Pointer());
}

void MainWindow::swapChainPanel_PointerMoved(IInspectable const& sender, PointerRoutedEventArgs const& e)
{
    auto pointerPoint = e.GetCurrentPoint(swapChainPanel());
    auto position = pointerPoint.Position();

    WPARAM btnState = 0;
    if (pointerPoint.Properties().IsLeftButtonPressed())
    {
        btnState |= MK_LBUTTON;
    }

    if (pointerPoint.Properties().IsRightButtonPressed())
    {
        btnState |= MK_RBUTTON;
    }

    auto& display = _d3dApp->GetDisplay();
    auto pt = display.ToScenePoint(position.X, position.Y);

    _Text->Text() = std::format(L"MousePosition X:{0:.2f} Y:{1:.2f}", pt.X(), pt.Y());

    _d3dApp->OnMouseMove(btnState, position.X, position.Y);
}

void MainWindow::Window_Activated(IInspectable const& sender, WindowActivatedEventArgs const& args)
{
    if (args.WindowActivationState() == WindowActivationState::Deactivated)
    {
        _Timer.Stop();
    }
    else if (
        args.WindowActivationState() == WindowActivationState::PointerActivated ||
        args.WindowActivationState() == WindowActivationState::CodeActivated)
    {
        _Timer.Start();
    }
}

void MainWindow::Window_Closed(IInspectable const& sender, WindowEventArgs const& args)
{
    _WindowClosed = true;
}

IAsyncAction MainWindow::btnLoadPolyline_Click(IInspectable const& sender, RoutedEventArgs const& e)
{
    winrt::Windows::Storage::Pickers::FileOpenPicker picker;
    auto windowNative{this->m_inner.as<IWindowNative>()};
    HWND hWnd{0};
    windowNative->get_WindowHandle(&hWnd);

    auto init_with_window = picker.as<IInitializeWithWindow>();
    winrt::check_hresult(init_with_window->Initialize(hWnd));

    picker.ViewMode(winrt::Windows::Storage::Pickers::PickerViewMode::Thumbnail);
    picker.FileTypeFilter().Append(L"*");

    StorageFile file = co_await picker.PickSingleFileAsync();
    if (!file)
    {
        co_return;
    }

    winrt::hstring jsonString;
    try
    {
        jsonString = co_await FileIO::ReadTextAsync(file);
        auto polyline = ::ShapeViewer::JsonHelper::ReadJson(jsonString.c_str());
        auto& display = _d3dApp->GetDisplay();
        auto roi = new ::ShapeViewer::ROIPolyline(polyline);
        display.AddROI(roi);
    }
    catch (const hresult_error& ex)
    {
        winrt::hresult hr = ex.code();
        winrt::hstring message = ex.message();
    }
}

void MainWindow::btnFit_Click(IInspectable const& sender, RoutedEventArgs const& e)
{
    _d3dApp->GetDisplay().FitSize();
}

void MainWindow::swapChainPanel_PointerWheelChanged(IInspectable const& sender, PointerRoutedEventArgs const& e)
{
    auto pointerPoint = e.GetCurrentPoint(swapChainPanel());
    auto position = pointerPoint.Position();

    _d3dApp->OnMouseWheel(0, position.X, position.Y, pointerPoint.Properties().MouseWheelDelta());
}
