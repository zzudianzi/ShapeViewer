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

bool ROIPolygon::PtInShape(const Point& pt) const
{
    throw;
}

bool ROIPolygon::PtNearBoundary(const Point& pt, double maxDis) const
{
    return Math::PointNearBoundary(pt, GetPolygon().GetVertices(), maxDis);
}

void ROIPolygon::DragMark(int selectedMark, const Point& oriPos, const Point& curPos, const ROI& oriROI)
{
    const auto& oriROIPolygon = dynamic_cast<const ROIPolygon&>(oriROI);
    auto markPositions = oriROIPolygon.ROI::MarkPositions();
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

bool ROIPolygon::Draw()
{
    auto display = GetVis().GetDisplay();
    auto brush = display->Brush();
    auto rt = display->RenderTarget();
    auto transform = display->TransformToWindow();
    _VisPolygon.Draw();
    if (_DrawMark)
    {
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

const Vis& ROIPolygon::GetVis() const
{
    return _VisPolygon;
}

Vis& ROIPolygon::GetVis()
{
    return _VisPolygon;
}
