#include "pch.h"
#include "MainWindowViewModel.h"
#include "MainWindowViewModel.g.cpp"

using namespace winrt::Microsoft::UI::Xaml::Data;

namespace winrt::ShapeViewer::implementation
{

MainWindowViewModel::MainWindowViewModel() : _Title(L"ShapeViewer"), _App(nullptr)
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

bool MainWindowViewModel::Show2D()
{
    return _Show2D;
}
void MainWindowViewModel::Show2D(bool value)
{
    if (_Show2D != value)
    {
        _Show2D = value;
        RaisePropertyChanged(L"Show2D");

        if (_App)
        {
            _App->Show2D(_Show2D);
        }
    }
}
bool MainWindowViewModel::Show3D()
{
    return _Show3D;
}
void MainWindowViewModel::Show3D(bool value)
{
    if (_Show3D != value)
    {
        _Show3D = value;
        RaisePropertyChanged(L"Show3D");
        
        if (_App)
        {
            _App->Show3D(_Show3D);
        }
    }
}

const ::ShapeViewer::D3DApp* MainWindowViewModel::App() const
{
    return _App;
}

::ShapeViewer::D3DApp* MainWindowViewModel::App()
{
    return _App;
}

void MainWindowViewModel::App(::ShapeViewer::D3DApp* value)
{
    _App = value;
}

} // namespace winrt::ShapeViewer::implementation
