#include "pch.h"
#include "ROI.h"

using namespace ShapeViewer;

ROI::ROI() : _Visible(true), _DrawMark(true)
{
}

bool ROI::DrawMark() const
{
    return _DrawMark;
}

void ROI::DrawMark(bool value)
{
    _DrawMark = value;
}

bool ROI::Visible() const
{
    return _Visible;
}

void ROI::Visible(bool value)
{
    _Visible = value;
}

bool ROI::Scaling() const
{
    return _Scaling;
}

void ROI::Scaling(bool value)
{
    _Scaling = value;
}

D2D1_COLOR_F ROI::MarkColor() const
{
    return _MarkColor;
}

void ROI::MarkColor(D2D1_COLOR_F value)
{
    _MarkColor = value;
}