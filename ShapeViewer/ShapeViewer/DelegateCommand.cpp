#include "pch.h"
#include "DelegateCommand.h"
#include "DelegateCommand.g.cpp"

using namespace winrt::Windows::Foundation;

namespace winrt::ShapeViewer::implementation
{

DelegateCommand::DelegateCommand(const ExecuteFunction& executeFunction) : DelegateCommand(executeFunction, nullptr)
{
}

DelegateCommand::DelegateCommand(const ExecuteFunction& executeFunction, const CanExecuteFunction& canExecuteFunction)
{
    if (executeFunction == nullptr)
    {
        throw hresult_invalid_argument(L"executeFunction");
    }

    _ExecuteDelegate = executeFunction;
    _CanExecuteDelegate = canExecuteFunction;
}

void DelegateCommand::RaiseCanExecuteChanged()
{
    _CanExecuteChanged(*this, IInspectable());
}

winrt::event_token DelegateCommand::CanExecuteChanged(EventHandler<IInspectable> const& handler)
{
    return _CanExecuteChanged.add(handler);
}

void DelegateCommand::CanExecuteChanged(winrt::event_token const& token) noexcept
{
    _CanExecuteChanged.remove(token);
}

bool DelegateCommand::CanExecute(IInspectable const& parameter)
{
    if (_CanExecuteDelegate == nullptr)
    {
        return true;
    }

    bool canExecute = _CanExecuteDelegate(parameter);

    if (_LastCanExecute != canExecute)
    {
        _LastCanExecute = canExecute;
        RaiseCanExecuteChanged();
    }

    return _LastCanExecute;
}

void DelegateCommand::Execute(IInspectable const& parameter)
{
    if (_ExecuteDelegate != nullptr)
    {
        _ExecuteDelegate(parameter);
    }
}

} // namespace winrt::ShapeViewer::implementation
