#include "pch.h"
#include "ROIArc.h"
#include "MathTool.h"
#include "TemplateHelper.hpp"
#include "Display.h"

using namespace ShapeViewer;

ROIArc::ROIArc(const ::ShapeViewer::Arc& arc) : _VisArc(arc)
{

}

ROIArc::ROIArc(const ::ShapeViewer::ROIArc& roi) : _VisArc(roi._VisArc)
{

}

::ShapeViewer::ROI* ROIArc::Clone() const
{
    return ::ShapeViewer::Clone(*this);
}

::ShapeViewer::Arc& ROIArc::GetArc()
{
    return _VisArc.Arc();
}

const ::ShapeViewer::Arc& ROIArc::GetArc() const
{
    return _VisArc.Arc();
}

::ShapeViewer::VisArc& ROIArc::GetVisArc()
{
    return _VisArc;
}

const ::ShapeViewer::VisArc& ROIArc::GetVisArc() const
{
    return _VisArc;
}

::ShapeViewer::Vis& ROIArc::GetVis()
{
    return _VisArc;
}

const ::ShapeViewer::Vis& ROIArc::GetVis() const
{
    return _VisArc;
}

::ShapeViewer::Point ROIArc::Center() const
{
    const auto& arc = _VisArc.Arc();

    auto pt = arc.PointAtRadian((arc.StartRadian() + arc.StopRadian()) * 0.5);
    if (arc.RestrictStartRadian() < arc.RestrictStopRadian())
    {
        return pt;
    }
    
    return arc.Center() * 2. - pt;
}

bool ROIArc::SamePos(const ROI& roi) const
{
    auto o = dynamic_cast<const ROIArc*>(&roi);
    if (o == nullptr)
    {
        return false;
    }

    if (o == this)
    {
        return true;
    }
    return GetArc() == o->GetArc();
}

void ROIArc::MarkPositions(std::vector<Point>& markPositions) const
{
    markPositions.resize((int)Mark::Count);
    const auto& arc = _VisArc.Arc();
    markPositions[(int)Mark::Start] = arc.StartPoint();
    markPositions[(int)Mark::Stop] = arc.StopPoint();

    auto centerInArc = Center();
    Point vec;
    Math::UnitVector(centerInArc - arc.Center(), vec);
    markPositions[(int)Mark::Move] = centerInArc - vec * MarkOffset;
    markPositions[(int)Mark::Roundness] = centerInArc + vec * MarkOffset;
}

bool ROIArc::PtInShape(const Point& pt) const
{
    return false;
}

bool ROIArc::PtNearBoundary(const Point& pt, double maxDis) const
{
    assert(maxDis > 0);
    if (maxDis < 0)
    {
        return false;
    }
    const auto& arc = _VisArc.Arc();
    auto radius = arc.Radius();
    auto minRadius = (std::max)(0., radius - maxDis);
    auto maxRadius = radius + maxDis;
    auto radianTolerance = maxDis / arc.Radius();

    auto center = arc.Center();
    auto dis = Math::CalcDistanceFormPointToPoint(pt, center);
    if (dis < minRadius || dis > maxRadius)
    {
        return false;
    }

    auto radian = std::atan2(pt.Y() - center.Y(), pt.X() - center.X());
    if (arc.RestrictStartRadian() < radian && radian < arc.RestrictStopRadian())
    {
        return true;
    }

    if (arc.RestrictStartRadian() > arc.RestrictStopRadian() && (arc.RestrictStartRadian() < radian || radian < arc.RestrictStopRadian()))
    {
        return true;
    }

    return false;
}

void ROIArc::DragMark(int selectedMark, const Point& oriPos, const Point& curPos, const ROI& oriROI)
{
    auto oriROIArc = dynamic_cast<const ROIArc*>(&oriROI);
    auto markPositions = oriROIArc->ROI::MarkPositions();
    switch ((Mark)selectedMark)
    {
    case Mark::Start:
        {
            auto center = oriROIArc->GetArc().Center();
            auto startRadian = Math::CalcRadianByTwoPoints(center, curPos);
            GetArc().StartRadian(Arc::RestrictRadian(startRadian));
        }
        break;
    case Mark::Stop:
        {
            auto center = oriROIArc->GetArc().Center();
            auto stopRadian = Math::CalcRadianByTwoPoints(center, curPos);
            GetArc().StopRadian(Arc::RestrictRadian(stopRadian));
        }
        break;
    case Mark::Move:
        {
            auto offset = curPos - markPositions[(int)Mark::Move];
            GetArc().Center() = oriROIArc->GetArc().Center() + offset;
        }
        break;
    case Mark::Roundness:
        break;
    default:
        break;
    }
}


bool ROIArc::Draw()
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
    GetVis().Draw();
    if (_DrawMark)
    {
        auto markPositions = ROI::MarkPositions();
        for (int i = 0; i < markPositions.size(); i++)
        {
            auto color = D2D1::ColorF(D2D1::ColorF::Yellow);
            if (i == (int)Mark::Roundness)
            {
                color = D2D1::ColorF(D2D1::ColorF::Green);
            }
            else if (i == (int)Mark::Move)
            {
                color = D2D1::ColorF(D2D1::ColorF::Blue);
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
