#include "pch.h"
#include "ROICircle.h"
#include "TemplateHelper.hpp"
#include "MathTool.h"
#include "Display.h"

using namespace ShapeViewer;

ROICircle::ROICircle(const ::ShapeViewer::Circle& circle) : _VisCircle(circle)
{
}

ROICircle::ROICircle(const ::ShapeViewer::ROICircle& roi) : _VisCircle(roi.GetVisCircle())
{
}

::ShapeViewer::Circle& ROICircle::GetCircle()
{
    return _VisCircle.Circle();
}

const ::ShapeViewer::Circle& ROICircle::GetCircle() const
{
    return _VisCircle.Circle();
}

::ShapeViewer::VisCircle& ROICircle::GetVisCircle()
{
    return _VisCircle;
}

const ::ShapeViewer::VisCircle& ROICircle::GetVisCircle() const
{
    return _VisCircle;
}

::ShapeViewer::Vis& ROICircle::GetVis()
{
    return _VisCircle;
}

const ::ShapeViewer::Vis& ROICircle::GetVis() const
{
    return _VisCircle;
}

ROI* ROICircle::Clone() const
{
    return ShapeViewer::Clone(*this);
}

::ShapeViewer::Point ROICircle::Center() const
{
    return GetCircle().Center();
}

bool ROICircle::SamePos(const ROI& roi) const
{
    auto obj = dynamic_cast<const ROICircle*>(&roi);
    if (obj == nullptr)
    {
        return false;
    }

    if (obj == this)
    {
        return true;
    }

    return GetCircle() == obj->GetCircle();
}

void ROICircle::MarkPositions(std::vector<Point>& markPositions) const
{
    markPositions.clear();
    markPositions.resize((int)Mark::Count);
    const auto& center = GetCircle().Center();
    auto radius = GetCircle().Radius();
    markPositions[(int)Mark::Center] = center;
    markPositions[(int)Mark::Left] = {center.X() - radius, Center().Y()};
    markPositions[(int)Mark::Top] = {center.X(), Center().Y() - radius};
    markPositions[(int)Mark::Right] = {center.X() + radius, Center().Y()};
    markPositions[(int)Mark::Bottom] = {center.X(), Center().Y() + radius};
}

bool ROICircle::PtInShape(const Point& pt) const
{
    return GetCircle().PtInCircle(pt);
}

bool ROICircle::PtNearBoundary(const Point& pt, double maxDis) const
{
    auto dis = Math::CalcDistanceFormPointToPoint(Center(), pt);

    return std::abs(dis - GetCircle().Radius()) < maxDis;
}

void ROICircle::DragMark(int selectedMark, const Point& oriPos, const Point& curPos, const ROI& oriROI)
{
    auto markPositions = oriROI.MarkPositions();
    auto disFunc = [this, &curPos]() { return Math::CalcDistanceFormPointToPoint(Center(), curPos); };
    switch ((Mark)selectedMark)
    {
    case Mark::Center:
        GetCircle().Center() = curPos;
        break;
    case Mark::Left:
    case Mark::Top:
    case Mark::Right:
    case Mark::Bottom:
        GetCircle().Radius(disFunc());
        break;
    default:
        break;
    }
}

bool ROICircle::Draw()
{
    auto display = GetVis().GetDisplay();
    auto brush = display->Brush();
    auto rt = display->RenderTarget();
    auto transform = display->TransformToWindow();
    GetVis().Draw();
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
