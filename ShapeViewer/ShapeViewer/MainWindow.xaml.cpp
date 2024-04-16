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

using namespace winrt;
using namespace Windows::Foundation;
using namespace Microsoft::UI::Input;
using namespace Microsoft::UI::Xaml;
using namespace Microsoft::UI::Xaml::Controls;
using namespace Microsoft::UI::Xaml::Data;
using namespace Microsoft::UI::Xaml::Media;
using namespace Microsoft::UI::Composition;
using namespace Windows::Storage;

// To learn more about WinUI, the WinUI project structure,
// and more about our project templates, see: http://aka.ms/winui-project-info.

namespace winrt::ShapeViewer::implementation
{
MainWindow::MainWindow()
{
    _ViewModel = winrt::make<winrt::ShapeViewer::implementation::MainWindowViewModel>();
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

        Title(windowText.c_str());

        frameCount = 0;
        timeEplapsed += 1.;
    }
}

} // namespace winrt::ShapeViewer::implementation

void winrt::ShapeViewer::implementation::MainWindow::swapChainPanel_Loaded(
    winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e)
{
    _d3dApp.reset(new BoxApp(swapChainPanel().as<ISwapChainPanelNative>()));
    _d3dApp->InitDirect3D();
    _d3dApp->OnResize(swapChainPanel().ActualWidth(), swapChainPanel().ActualHeight());

    _RenderingToken = CompositionTarget::Rendering({this, &MainWindow::swapChainPanel_RenderingHandler});
}

void winrt::ShapeViewer::implementation::MainWindow::swapChainPanel_Unloaded(
    winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e)
{
    _d3dApp.reset(nullptr);
    CompositionTarget::Rendering(_RenderingToken);
}

void winrt::ShapeViewer::implementation::MainWindow::swapChainPanel_SizeChanged(
    winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::SizeChangedEventArgs const& e)
{
    if (swapChainPanel().IsLoaded())
    {
        winrt::Windows::Foundation::Size size = e.NewSize();
        _d3dApp->OnResize(size.Width, size.Height);
    }
}

void winrt::ShapeViewer::implementation::MainWindow::swapChainPanel_RenderingHandler(
    winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::Foundation::IInspectable const& args)
{
    if (_WindowClosed || !_d3dApp)
    {
        return;
    }
    CalculateFrameStats();
    _d3dApp->Update();
    _d3dApp->Draw();
}

void winrt::ShapeViewer::implementation::MainWindow::swapChainPanel_PointerPressed(
    winrt::Windows::Foundation::IInspectable const& sender,
    winrt::Microsoft::UI::Xaml::Input::PointerRoutedEventArgs const& e)
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

void winrt::ShapeViewer::implementation::MainWindow::swapChainPanel_PointerReleased(
    winrt::Windows::Foundation::IInspectable const& sender,
    winrt::Microsoft::UI::Xaml::Input::PointerRoutedEventArgs const& e)
{
    swapChainPanel().ReleasePointerCapture(e.Pointer());
}

void winrt::ShapeViewer::implementation::MainWindow::swapChainPanel_PointerMoved(
    winrt::Windows::Foundation::IInspectable const& sender,
    winrt::Microsoft::UI::Xaml::Input::PointerRoutedEventArgs const& e)
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

    _d3dApp->OnMouseMove(btnState, position.X, position.Y);
}

void winrt::ShapeViewer::implementation::MainWindow::Window_Activated(
    winrt::Windows::Foundation::IInspectable const& sender,
    winrt::Microsoft::UI::Xaml::WindowActivatedEventArgs const& args)
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

void winrt::ShapeViewer::implementation::MainWindow::Window_Closed(
    winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::WindowEventArgs const& args)
{
    _WindowClosed = true;
}

IAsyncAction winrt::ShapeViewer::implementation::MainWindow::btnLoadPolyline_Click(
    winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e)
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

    winrt::hstring jsonString = co_await FileIO::ReadTextAsync(file);

    ::ShapeViewer::JsonHelper::ReadJson(jsonString.c_str());
}
