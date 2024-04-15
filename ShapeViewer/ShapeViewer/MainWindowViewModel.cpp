#include "pch.h"
#include "MainWindowViewModel.h"
#include "MainWindowViewModel.g.cpp"

using namespace winrt::Microsoft::UI::Xaml::Data;

namespace winrt::ShapeViewer::implementation
{

MainWindowViewModel::MainWindowViewModel() : _Title(L"ShapeViewer")
{
}

hstring MainWindowViewModel::Title()
{
    return _Title;
}

void MainWindowViewModel::Title(hstring const& value)
{
    if (_Title != value)
    {
        _Title = value;
        RaisePropertyChanged(L"Title");
    }
}

} // namespace winrt::ShapeViewer::implementation
