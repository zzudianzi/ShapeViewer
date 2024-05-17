#pragma once

#include "MainWindowViewModel.g.h"
#include "BindableBase.h"
#include "d3dApp.h"

namespace winrt::ShapeViewer::implementation
{
struct MainWindowViewModel : MainWindowViewModelT<MainWindowViewModel, ShapeViewer::implementation::BindableBase>
{
    MainWindowViewModel();

    hstring Title();
    void Title(hstring const& value);

    bool Show2D();
    void Show2D(bool value);

    bool Show3D();
    void Show3D(bool value);

    const ::ShapeViewer::D3DApp* App() const;
    ::ShapeViewer::D3DApp* App();
    void App(::ShapeViewer::D3DApp* value);

  private:
    winrt::hstring _Title;
    bool _Show2D;
    bool _Show3D;

    ::ShapeViewer::D3DApp* _App;
};
} // namespace winrt::ShapeViewer::implementation

namespace winrt::ShapeViewer::factory_implementation
{
struct MainWindowViewModel : MainWindowViewModelT<MainWindowViewModel, implementation::MainWindowViewModel>
{
};
} // namespace winrt::ShapeViewer::factory_implementation
