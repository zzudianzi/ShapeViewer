#include "pch.h"
#include "ROIPolygon.h"
#include "MathTool.h"

#include <algorithm>

#include "Display.h"

using namespace ShapeViewer;

ROIPolygon::ROIPolygon(const Polygon& polygon) : _VisPolygon(polygon)
{
}

ROIPolygon::ROIPolygon(const ShapeViewer::VisPolygon& visPolygon) : _VisPolygon(visPolygon)
{
}

ROIPolygon::ROIPolygon(const ROIPolygon& roiPolygon) : _VisPolygon(roiPolygon.GetVisPolygon())
{

}

ROI* ROIPolygon::Clone() const
{
    return new ROIPolygon(*this);
}

const ShapeViewer::Polygon& ROIPolygon::GetPolygon() const
{
    return _VisPolygon.Polygon();
}

ShapeViewer::Polygon& ROIPolygon::GetPolygon()
{
    return _VisPolygon.Polygon();
}

const ShapeViewer::VisPolygon& ROIPolygon::GetVisPolygon() const
{
    return _VisPolygon;
}

ShapeViewer::VisPolygon& ROIPolygon::GetVisPolygon()
{
    return _VisPolygon;
}

Point ROIPolygon::Center() const
{
    return GetPolygon().GetCenter();
}

bool ROIPolygon::SamePos(const ROI& roi) const
{
    auto polygon = dynamic_cast<const ROIPolygon*>(&roi);
    if (polygon == nullptr)
    {
        return false;
    }

    if (polygon == this)
    {
        return true;
    }

    return GetPolygon() == polygon->GetPolygon();
}

void ROIPolygon::MarkPositions(std::vector<Point>& markPositions) const
{
    markPositions.clear();
    markPositions.push_back(GetPolygon().GetCenter());
    for (auto&& vertex : GetPolygon().GetVertices())
    {
        markPositions.push_back(vertex);
    }
}

std::vector<Point> ROIPolygon::MarkPositions() const
{
    std::vector<Point> markPositions;
    MarkPositions(markPositions);
    return markPositions;
}

bool ROIPolygon::PtInShape(const Point& pt) const
{
    throw;
}

bool ROIPolygon::PtNearBoundary(const Point& pt, double maxDis) const
{
    const auto& vertices = GetPolygon().GetVertices();
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

void ROIPolygon::DragMark(int selectedMark, const Point& oriPos, const Point& curPos, const ROI& oriROI)
{
    const auto& oriROIPolygon = dynamic_cast<const ROIPolygon&>(oriROI);
    auto markPositions = oriROIPolygon.MarkPositions();
    switch ((Mark)selectedMark)
    {
    case Mark::Center:
        {
            auto originCenter = oriROIPolygon.Center();
            auto offset = curPos - originCenter;
            auto& vertices = GetPolygon().GetVertices();
            int count = (int)vertices.size();
            for (int i = 0; i < count; i++)
            {
                vertices[i] = oriROIPolygon.GetPolygon().GetCorner(i) + offset;
            }
        }
        break;
    default:
        {
            int index = selectedMark - 1;
            auto& vertices = GetPolygon().GetVertices();
            if (index >= 0 && index < (int)vertices.size())
            {
                vertices[index] = curPos;
            }
        }
        break;
    }
}

bool ROIPolygon::Draw(ID2D1RenderTarget* rt, ID2D1SolidColorBrush* brush, const D2D1::Matrix3x2F& transform)
{
    _VisPolygon.Draw(rt, brush);
    if (_DrawMark)
    {
        brush->SetColor(D2D1::ColorF(D2D1::ColorF::Yellow));
        auto markPositions = MarkPositions();
        for (auto&& pos : markPositions)
        {
            auto center =
                D2D1::Ellipse(transform.TransformPoint(D2D1::Point2F((float)pos.X(), (float)pos.Y())), MarkRadius, MarkRadius);
            rt->FillEllipse(center, brush);
        }
    }

    return true;
}

const Vis& ROIPolygon::GetVis() const
{
    return _VisPolygon;
}

Vis& ROIPolygon::GetVis()
{
    return _VisPolygon;
}
