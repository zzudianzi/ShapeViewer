#include "pch.h"
#include "Vis.h"
#include <d2d1helper.h>

using namespace ShapeViewer;

Vis::Vis() : _Visible(true)
{
    _Color = D2D1::ColorF(D2D1::ColorF::SkyBlue);
}

D2D1_COLOR_F Vis::Color() const
{
    return _Color;
}

void Vis::Color(D2D1_COLOR_F value)
{
    _Color = value;
}
