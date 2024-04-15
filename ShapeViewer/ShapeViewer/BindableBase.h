#pragma once
#include "BindableBase.g.h"

namespace winrt::ShapeViewer::implementation
{
struct BindableBase : BindableBaseT<BindableBase>
{
    BindableBase() = default;

    void RaisePropertyChanged(hstring const& propertyName);
    winrt::event_token PropertyChanged(winrt::Microsoft::UI::Xaml::Data::PropertyChangedEventHandler const& handler);
    void PropertyChanged(winrt::event_token const& token) noexcept;

  private:
    event<winrt::Microsoft::UI::Xaml::Data::PropertyChangedEventHandler> _PropertyChanged;
};
} // namespace winrt::ShapeViewer::implementation
namespace winrt::ShapeViewer::factory_implementation
{
struct BindableBase : BindableBaseT<BindableBase, implementation::BindableBase>
{
};
} // namespace winrt::ShapeViewer::factory_implementation
