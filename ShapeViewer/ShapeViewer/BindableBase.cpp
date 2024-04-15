#include "pch.h"
#include "BindableBase.h"
#include "BindableBase.g.cpp"

using namespace winrt::Microsoft::UI::Xaml::Data;

namespace winrt::ShapeViewer::implementation
{
void BindableBase::RaisePropertyChanged(hstring const& propertyName)
{
    _PropertyChanged(*this, PropertyChangedEventArgs(propertyName));
}
winrt::event_token BindableBase::PropertyChanged(
    winrt::Microsoft::UI::Xaml::Data::PropertyChangedEventHandler const& handler)
{
    return _PropertyChanged.add(handler);
}
void BindableBase::PropertyChanged(winrt::event_token const& token) noexcept
{
    _PropertyChanged.remove(token);
}
} // namespace winrt::ShapeViewer::implementation
