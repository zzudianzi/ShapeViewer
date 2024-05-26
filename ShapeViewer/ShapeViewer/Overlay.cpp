#include "pch.h"
#include "Overlay.h"

using namespace ShapeViewer;

Overlay::Overlay(Display* display, bool scaling) : _Display(display), _Scaling(scaling)
{
}

Overlay::~Overlay()
{
    DeleteAllItems();
}

Display* Overlay::GetDisplay()
{
    return _Display;
}

const Display* Overlay::GetDisplay() const
{
    return _Display;
}

void Overlay::SetDisplay(Display* display)
{
    _Display = display;
}

bool Overlay::Visible() const
{
    return _Visible;
}

void Overlay::Visible(bool value)
{
    _Visible = value;
}

bool Overlay::Scaling() const
{
    return _Scaling;
}

void Overlay::Scaling(bool value)
{
    _Scaling = value;
}

void Overlay::AddItem(Geometry* geometry)
{
    throw;
}

void Overlay::AddItem(Vis* vis)
{
    _Items.push_back(vis);
    vis->SetDisplay(_Display);
}

Vis* Overlay::GetItem(int index)
{
    if (index < 0 || index >= (int)_Items.size())
    {
        return nullptr;
    }
    return _Items[index];
}

bool Overlay::DeleteItem(Vis* vis)
{
    if (!vis)
    {
        return false;
    }

    auto iter = std::find(_Items.begin(), _Items.end(), vis);
    if (iter == _Items.end())
    {
        return false;
    }

    _Items.erase(iter);
    delete vis;
    return true;
}

bool Overlay::DeleteItem(int index)
{
    return DeleteItem(GetItem(index));
}

void Overlay::DeleteAllItems()
{
    for (auto vis : _Items)
    {
        delete vis;
    }

    _Items.clear();
}

int Overlay::GetItemCount()
{
    return (int)_Items.size();
}

void Overlay::Draw()
{
    for (auto&& vis : _Items)
    {
        vis->Draw();
    }
}
