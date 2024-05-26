#include "pch.h"
#include "ROIPoint.h"
#include "TemplateHelper.hpp"
#include "MathTool.h"
#include "Display.h"

using namespace ShapeViewer;

ROIPoint::ROIPoint(const ::ShapeViewer::Point& point) : _VisPoint(point)
{
}

ROIPoint::ROIPoint(const ROIPoint& roi) : _VisPoint(_VisPoint)
{
}

ROI* ROIPoint::Clone() const
{
    return ::ShapeViewer::Clone(*this);
}

::ShapeViewer::Point& ROIPoint::GetPoint()
{
    return _VisPoint.Point();
}
const ::ShapeViewer::Point& ROIPoint::GetPoint() const
{
    return _VisPoint.Point();
}

::ShapeViewer::VisPoint& ROIPoint::GetVisPoint()
{
    return _VisPoint;
}
const ::ShapeViewer::VisPoint& ROIPoint::GetVisPoint() const
{
    return _VisPoint;
}

::ShapeViewer::Vis& ROIPoint::GetVis()
{
    return _VisPoint;
}
const ::ShapeViewer::Vis& ROIPoint::GetVis() const
{
    return _VisPoint;
}

::ShapeViewer::Point ROIPoint::Center() const
{
    return _VisPoint.Point();
}

bool ROIPoint::SamePos(const ROI& roi) const
{
    auto o = dynamic_cast<const ROIPoint*>(&roi);
    if (o == nullptr)
    {
        return false;
    }

    if (o == this)
    {
        return true;
    }
    return GetPoint() == o->GetPoint();
}
void ROIPoint::MarkPositions(std::vector<Point>& markPositions) const
{
    markPositions.push_back(GetPoint());
}
std::vector<Point> ROIPoint::MarkPositions() const
{
    std::vector<Point> markPositions;
    MarkPositions(markPositions);
    return markPositions;
}
bool ROIPoint::PtInShape(const Point& pt) const
{
    return false;
}
bool ROIPoint::PtNearBoundary(const Point& pt, double maxDis) const
{
    return Math::CalcDistanceFormPointToPoint(_VisPoint.Point(), pt) < maxDis;
}
void ROIPoint::DragMark(int selectedMark, const Point& oriPos, const Point& curPos, const ROI& oriROI)
{
    switch ((Mark)selectedMark)
    {
    case Mark::Center:
        GetPoint() = curPos;
        break;
    default:
        assert(false);
        break;
    }
}

bool ROIPoint::Draw()
{
    GetVis().Draw();
    if (_DrawMark)
    {
        auto display = GetVis().GetDisplay();
        assert(display);
        if (!display)
        {
            return false;
        }
        auto brush = display->Brush();
        auto rt = display->RenderTarget();
        auto transform = display->TransformToWindow();
        brush->SetColor(D2D1::ColorF(D2D1::ColorF::Yellow));

        auto markPositions = MarkPositions();
        for (int i = 0; i < markPositions.size(); i++)
        {
            auto color = D2D1::ColorF(D2D1::ColorF::Yellow);
            brush->SetColor(color);
            auto center = D2D1::Ellipse(
                transform.TransformPoint(D2D1::Point2F((float)markPositions[i].X(), (float)markPositions[i].Y())),
                MarkRadius,
                MarkRadius);
            rt->FillEllipse(center, brush.get());
        }
    }

    return true;
}
