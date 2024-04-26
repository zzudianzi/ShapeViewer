#include "pch.h"
#include "Display.h"

using namespace ShapeViewer;

Display::Display() : _Scale(1.)
{

}

Display::~Display()
{
    DeleteAllOverlays();
}

void Display::SetSceneRect(const Rect& rc)
{
    throw;
}

bool Display::FitSize(bool width, bool height)
{
    throw;
}

Overlay* Display::CreateOverlay(bool scaling)
{
    throw;
}

bool Display::DeleteOverlay(Overlay* overlay)
{
    if (!overlay)
    {
        return false;
    }

    auto iter = std::find(_Overlays.begin(), _Overlays.end(), overlay);
    if (iter == _Overlays.end())
    {
        return false;
    }

    delete overlay;
    _Overlays.erase(iter);
    return true;
}

bool Display::DeleteOverlay(int index)
{
    return DeleteOverlay(GetOverlay(index));
}

void Display::DeleteAllOverlays()
{
    for (auto overlay : _Overlays)
    {
        delete overlay;
    }

    _Overlays.clear();
}

int Display::GetOverlayCount() const
{
    return (int)_Overlays.size();
}

Overlay* Display::GetOverlay(int index)
{
    if (index < 0 || index >= _Overlays.size())
    {
        return nullptr;
    }

    return _Overlays[index];
}
