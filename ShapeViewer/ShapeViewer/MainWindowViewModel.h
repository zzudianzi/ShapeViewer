#pragma once

#include "MainWindowViewModel.g.h"
#include "BindableBase.h"

namespace winrt::ShapeViewer::implementation
{
struct MainWindowViewModel : MainWindowViewModelT<MainWindowViewModel, ShapeViewer::implementation::BindableBase>
{
    MainWindowViewModel();

    hstring Title();
    void Title(hstring const& value);

  private:
    winrt::hstring _Title;
};
} // namespace winrt::ShapeViewer::implementation

namespace winrt::ShapeViewer::factory_implementation
{
struct MainWindowViewModel : MainWindowViewModelT<MainWindowViewModel, implementation::MainWindowViewModel>
{
};
} // namespace winrt::ShapeViewer::factory_implementation
