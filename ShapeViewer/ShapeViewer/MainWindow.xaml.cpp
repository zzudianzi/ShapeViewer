#include "pch.h"
#include "MainWindow.xaml.h"
#if __has_include("MainWindow.g.cpp")
#include "MainWindow.g.cpp"
#endif

#include "BoxApp.h"
#include <winrt/Microsoft.UI.Input.h>

using namespace winrt;
using namespace Microsoft::UI::Input;
using namespace Microsoft::UI::Xaml;
using namespace Microsoft::UI::Xaml::Media;
using namespace Microsoft::UI::Composition;

// To learn more about WinUI, the WinUI project structure,
// and more about our project templates, see: http://aka.ms/winui-project-info.

namespace winrt::ShapeViewer::implementation
{

MainWindow::~MainWindow() = default;

void MainWindow::InitializeComponent()
{
    MainWindowT::InitializeComponent();
}

} // namespace winrt::ShapeViewer::implementation

void winrt::ShapeViewer::implementation::MainWindow::swapChainPanel_Loaded(
    winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e)
{
    _d3dApp.reset(new BoxApp(swapChainPanel().as<ISwapChainPanelNative>()));
    _d3dApp->InitDirect3D();
    _d3dApp->OnResize(swapChainPanel().ActualWidth(), swapChainPanel().ActualHeight());

    CompositionTarget::Rendering({this, &MainWindow::swapChainPanel_RenderingHandler});
}

void winrt::ShapeViewer::implementation::MainWindow::swapChainPanel_Unloaded(
    winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e)
{
    _d3dApp.reset(nullptr);
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
    if (!_d3dApp)
    {
        return;
    }
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

    WPARAM btnState;
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

    WPARAM btnState;
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
