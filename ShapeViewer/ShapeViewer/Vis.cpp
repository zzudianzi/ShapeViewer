#include "pch.h"
#include "Vis.h"
#include "Display.h"
#include <d2d1helper.h>

using namespace ShapeViewer;

Vis::Vis() : _Visible(true), _Display(nullptr)
{
    _Color = D2D1::ColorF(D2D1::ColorF::SkyBlue);
}

Vis::Vis(const Vis& vis) : _Visible(vis._Visible), _Display(vis.GetDisplay()), _Color(vis.Color())
{

}

bool Vis::Visible() const
{
    return _Visible;
}

void Vis::Visible(bool value)
{
    _Visible = value;
}

D2D1_COLOR_F Vis::Color() const
{
    return _Color;
}

void Vis::Color(D2D1_COLOR_F value)
{
    _Color = value;
}

Display* Vis::GetDisplay()
{
    return _Display;
}

Display* Vis::GetDisplay() const
{
    return _Display;
}

void Vis::SetDisplay(Display* display)
{
    _Display = display;
}
