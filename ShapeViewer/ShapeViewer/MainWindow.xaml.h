#pragma once

#include "MainWindow.g.h"

#include "d3dApp.h"
#include "GameTimer.h"

namespace winrt::ShapeViewer::implementation
{
struct MainWindow : MainWindowT<MainWindow>
{
    MainWindow();
    ~MainWindow();

    void InitializeComponent();

    void CalculateFrameStats();

    winrt::ShapeViewer::MainWindowViewModel ViewModel();

  private:
    std::unique_ptr<D3DApp> _d3dApp;
    winrt::event_token _RenderingToken;
    GameTimer _Timer;

    winrt::hstring _WindowTitle; 

    bool _WindowClosed = false;

    winrt::ShapeViewer::MainWindowViewModel _ViewModel{nullptr};

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
    void Window_Activated(
        winrt::Windows::Foundation::IInspectable const& sender,
        winrt::Microsoft::UI::Xaml::WindowActivatedEventArgs const& args);
    void Window_Closed(
        winrt::Windows::Foundation::IInspectable const& sender,
        winrt::Microsoft::UI::Xaml::WindowEventArgs const& args);
};
} // namespace winrt::ShapeViewer::implementation

namespace winrt::ShapeViewer::factory_implementation
{
struct MainWindow : MainWindowT<MainWindow, implementation::MainWindow>
{
};
} // namespace winrt::ShapeViewer::factory_implementation
