#pragma once

#include "MainWindow.g.h"

#include "d3dApp.h"

namespace winrt::ShapeViewer::implementation
{
struct MainWindow : MainWindowT<MainWindow>
{
    MainWindow()
    {
        // Xaml objects should not call InitializeComponent during construction.
        // See https://github.com/microsoft/cppwinrt/tree/master/nuget#initializecomponent
    }

    ~MainWindow();

    void InitializeComponent();

  private:
    std::unique_ptr<D3DApp> _d3dApp;
    winrt::event_token _RenderingToken;

  public:
    void swapChainPanel_Loaded(
        winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e);
    void swapChainPanel_Unloaded(
        winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e);
    void swapChainPanel_SizeChanged(
        winrt::Windows::Foundation::IInspectable const& sender,
        winrt::Microsoft::UI::Xaml::SizeChangedEventArgs const& e);

    void swapChainPanel_RenderingHandler(
        winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::Foundation::IInspectable const& args);
    void swapChainPanel_PointerPressed(
        winrt::Windows::Foundation::IInspectable const& sender,
        winrt::Microsoft::UI::Xaml::Input::PointerRoutedEventArgs const& e);
    void swapChainPanel_PointerReleased(
        winrt::Windows::Foundation::IInspectable const& sender,
        winrt::Microsoft::UI::Xaml::Input::PointerRoutedEventArgs const& e);
    void swapChainPanel_PointerMoved(
        winrt::Windows::Foundation::IInspectable const& sender,
        winrt::Microsoft::UI::Xaml::Input::PointerRoutedEventArgs const& e);
};
} // namespace winrt::ShapeViewer::implementation

namespace winrt::ShapeViewer::factory_implementation
{
struct MainWindow : MainWindowT<MainWindow, implementation::MainWindow>
{
};
} // namespace winrt::ShapeViewer::factory_implementation
