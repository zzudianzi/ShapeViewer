#pragma once
#include "DelegateCommand.g.h"

#include <functional>

namespace winrt::ShapeViewer::implementation
{
struct DelegateCommand : DelegateCommandT<DelegateCommand>
{
    using ExecuteFunction = std::function<void(winrt::Windows::Foundation::IInspectable)>;
    using CanExecuteFunction = std::function<bool(winrt::Windows::Foundation::IInspectable)>;

    DelegateCommand(const ExecuteFunction& executeFunction);
    DelegateCommand(const ExecuteFunction& executeFunction, const CanExecuteFunction& canExecuteFunction);

    void RaiseCanExecuteChanged();
    winrt::event_token CanExecuteChanged(
        winrt::Windows::Foundation::EventHandler<winrt::Windows::Foundation::IInspectable> const& handler);
    void CanExecuteChanged(winrt::event_token const& token) noexcept;
    bool CanExecute(winrt::Windows::Foundation::IInspectable const& parameter);
    void Execute(winrt::Windows::Foundation::IInspectable const& parameter);

  private:
    event<winrt::Windows::Foundation::EventHandler<winrt::Windows::Foundation::IInspectable>> _CanExecuteChanged;

    ExecuteFunction _ExecuteDelegate;
    CanExecuteFunction _CanExecuteDelegate;

    bool _LastCanExecute = true;
};
} // namespace winrt::ShapeViewer::implementation
