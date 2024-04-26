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
