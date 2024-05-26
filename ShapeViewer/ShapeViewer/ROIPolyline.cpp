#include "pch.h"
#include "ROIPolyline.h"
#include "MathTool.h"

#include <algorithm>

#include "Display.h"

using namespace ShapeViewer;

ROIPolyline::ROIPolyline(const Polyline& Polyline) : _VisPolyline(Polyline)
{
}

ROIPolyline::ROIPolyline(const ShapeViewer::VisPolyline& visPolyline) : _VisPolyline(visPolyline)
{
}

ROIPolyline::ROIPolyline(const ROIPolyline& roiPolyline) : _VisPolyline(roiPolyline.GetVisPolyline())
{
}

ROI* ROIPolyline::Clone() const
{
    return new ROIPolyline(*this);
}

const ShapeViewer::Polyline& ROIPolyline::GetPolyline() const
{
    return _VisPolyline.Polyline();
}

ShapeViewer::Polyline& ROIPolyline::GetPolyline()
{
    return _VisPolyline.Polyline();
}

const ShapeViewer::VisPolyline& ROIPolyline::GetVisPolyline() const
{
    return _VisPolyline;
}

ShapeViewer::VisPolyline& ROIPolyline::GetVisPolyline()
{
    return _VisPolyline;
}

Point ROIPolyline::Center() const
{
    return GetPolyline().GetCenter();
}

bool ROIPolyline::SamePos(const ROI& roi) const
{
    auto Polyline = dynamic_cast<const ROIPolyline*>(&roi);
    if (Polyline == nullptr)
    {
        return false;
    }

    if (Polyline == this)
    {
        return true;
    }

    return GetPolyline() == Polyline->GetPolyline();
}

void ROIPolyline::MarkPositions(std::vector<Point>& markPositions) const
{
    markPositions.clear();
    markPositions.push_back(GetPolyline().GetCenter());
    for (auto&& vertex : GetPolyline().GetVertices())
    {
        markPositions.push_back(vertex);
    }
}

bool ROIPolyline::PtInShape(const Point& pt) const
{
    throw;
}

bool ROIPolyline::PtNearBoundary(const Point& pt, double maxDis) const
{
    const auto& vertices = GetPolyline().GetVertices();
    int size = (int)vertices.size();
    for (int i = 0; i < size; i++)
    {
        const auto& pt1 = vertices[(i + size - 1) % size];
        const auto& pt2 = vertices[i];
        if (Math::PointNearLineSegment(pt, pt1, pt2, maxDis))
        {
            return true;
        }
    }

    return false;
}

void ROIPolyline::DragMark(int selectedMark, const Point& oriPos, const Point& curPos, const ROI& oriROI)
{
    const auto& oriROIPolyline = dynamic_cast<const ROIPolyline&>(oriROI);
    auto markPositions = oriROIPolyline.ROI::MarkPositions();
    switch ((Mark)selectedMark)
    {
    case Mark::Center:
        {
            auto originCenter = oriROIPolyline.Center();
            auto offset = curPos - originCenter;
            auto& vertices = GetPolyline().GetVertices();
            int count = (int)vertices.size();
            for (int i = 0; i < count; i++)
            {
                vertices[i] = oriROIPolyline.GetPolyline().GetCorner(i) + offset;
            }
        }
        break;
    default:
        {
            int index = selectedMark - 1;
            auto& vertices = GetPolyline().GetVertices();
            if (index >= 0 && index < (int)vertices.size())
            {
                vertices[index] = curPos;
            }
        }
        break;
    }
}

bool ROIPolyline::Draw()
{
    auto display = GetVis().GetDisplay();
    auto brush = display->Brush();
    auto rt = display->RenderTarget();
    auto transform = display->TransformToWindow();
    _VisPolyline.Draw();
    if (_DrawMark)
    {
        brush->SetColor(D2D1::ColorF(D2D1::ColorF::Yellow));
        auto markPositions = ROI::MarkPositions();
        for (int i = 0; i < markPositions.size(); i++)
        {
            auto color = D2D1::ColorF(D2D1::ColorF::Yellow);
            if (i == (int)Mark::Center)
            {
                color = D2D1::ColorF(D2D1::ColorF::Green);
            }
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

const Vis& ROIPolyline::GetVis() const
{
    return _VisPolyline;
}

Vis& ROIPolyline::GetVis()
{
    return _VisPolyline;
}
