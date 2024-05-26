#include "pch.h"
#include "ROIEllipse.h"
#include "TemplateHelper.hpp"
#include "MathTool.h"
#include "Display.h"
#include "Line.h"

#include <numbers>

using namespace ShapeViewer;

ROIEllipse::ROIEllipse(const ::ShapeViewer::Ellipse& ellipse) : _VisEllipse(ellipse), _CanRotate(true)
{
}

ROIEllipse::ROIEllipse(const ::ShapeViewer::ROIEllipse& roi) : _VisEllipse(roi.GetVisEllipse())
{
}

::ShapeViewer::Ellipse& ROIEllipse::GetEllipse()
{
    return _VisEllipse.Ellipse();
}

const ::ShapeViewer::Ellipse& ROIEllipse::GetEllipse() const
{
    return _VisEllipse.Ellipse();
}

::ShapeViewer::VisEllipse& ROIEllipse::GetVisEllipse()
{
    return _VisEllipse;
}

const ::ShapeViewer::VisEllipse& ROIEllipse::GetVisEllipse() const
{
    return _VisEllipse;
}

::ShapeViewer::Vis& ROIEllipse::GetVis()
{
    return _VisEllipse;
}

const ::ShapeViewer::Vis& ROIEllipse::GetVis() const
{
    return _VisEllipse;
}

ROI* ROIEllipse::Clone() const
{
    return ShapeViewer::Clone(*this);
}

::ShapeViewer::Point ROIEllipse::Center() const
{
    return GetEllipse().Center();
}

bool ROIEllipse::CanRotate() const
{
    return _CanRotate;
}

void ROIEllipse::CanRotate(bool value)
{
    _CanRotate = value;
}

bool ROIEllipse::SamePos(const ROI& roi) const
{
    auto obj = dynamic_cast<const ROIEllipse*>(&roi);
    if (obj == nullptr)
    {
        return false;
    }

    if (obj == this)
    {
        return true;
    }

    return GetEllipse() == obj->GetEllipse();
}

void ROIEllipse::MarkPositions(std::vector<Point>& markPositions) const
{
    markPositions.clear();
    markPositions.resize((int)Mark::Count);
    auto boundingBox = GetVis().BoundingRect();
    if (!boundingBox)
    {
        return;
    }
    auto corners = boundingBox->Corners();
    markPositions[static_cast<int>(Mark::Center)] = GetEllipse().Center();
    markPositions[static_cast<int>(Mark::Left)] =
        (corners[static_cast<int>(Rect::Corner::TopLeft)] + corners[static_cast<int>(Rect::Corner::BottomLeft)]) * 0.5;
    markPositions[static_cast<int>(Mark::Top)] =
        (corners[static_cast<int>(Rect::Corner::TopLeft)] + corners[static_cast<int>(Rect::Corner::TopRight)]) * 0.5;
    markPositions[static_cast<int>(Mark::Right)] =
        (corners[static_cast<int>(Rect::Corner::TopRight)] + corners[static_cast<int>(Rect::Corner::BottomRight)]) *
        0.5;
    markPositions[static_cast<int>(Mark::Bottom)] =
        (corners[static_cast<int>(Rect::Corner::BottomLeft)] + corners[static_cast<int>(Rect::Corner::BottomRight)]) *
        0.5;
}

bool ROIEllipse::PtInShape(const Point& pt) const
{
    return GetEllipse().PtInEllipse(pt);
}

bool ROIEllipse::PtNearBoundary(const Point& pt, double maxDis) const
{
    const auto& ellipse = GetEllipse();
    auto a = ellipse.A();
    auto b = ellipse.B();
    auto c = std::sqrt(std::abs(a * a - b * b));
    auto angle = ellipse.Angle();
    auto center = ellipse.Center();
    if (a < b)
    {
        angle += std::numbers::pi * 0.5;
        std::swap(a, b);
    }
    auto vector = Point(std::cos(angle), sin(angle)) * c;
    auto focus1 = center - vector, focus2 = center + vector;
    
    auto dis1 = Math::CalcDistanceFormPointToPoint(pt, focus1);
    auto dis2 = Math::CalcDistanceFormPointToPoint(pt, focus2);

    return std::abs(dis1 + dis2 - 2. * a) < maxDis;
}

void ROIEllipse::DragMark(int selectedMark, const Point& oriPos, const Point& curPos, const ROI& oriROI)
{
    auto oriROIEllipse = dynamic_cast<const ROIEllipse*>(&oriROI);
    assert(oriROIEllipse);
    if (!oriROIEllipse)
    {
        return;
    }
    auto oriBoundingBox = oriROIEllipse->GetVisEllipse().BoundingRect();
    assert(oriBoundingBox);
    if (!oriBoundingBox)
    {
        return;
    }
    auto markPositions = oriROIEllipse->ROI::MarkPositions();
    switch ((Mark)selectedMark)
    {
    case Mark::Center:
        GetEllipse().Center() = curPos;
        break;
    case Mark::Left:
        {
            const auto& leftCenter = markPositions[static_cast<int>(Mark::Left)];
            const auto& rightCenter = markPositions[static_cast<int>(Mark::Right)];
            Line line(1, 1, 0);
            if (Math::CalcLineByTwoPoints(leftCenter, rightCenter, line))
            {
                Point foot;
                Math::CalcVerticalFootOfPointToline(curPos, line, foot);
                auto& ellipse = GetEllipse();
                ellipse.Center() = (foot + rightCenter) * 0.5;
                auto a = Math::CalcDistanceFormPointToPoint(curPos, rightCenter) * 0.5;
                ellipse.A(a);
            }
        }
        break;
    case Mark::Top:
        {
            const auto& topCenter = markPositions[static_cast<int>(Mark::Top)];
            const auto& bottomCenter = markPositions[static_cast<int>(Mark::Bottom)];
            double a, b, c;
            if (Math::CalcLineByTwoPoints(topCenter, bottomCenter, a, b, c))
            {
                Point foot;
                Math::CalcVerticalFootOfPointToline(curPos, a, b, c, foot);
                auto& ellipse = GetEllipse();
                ellipse.Center() = (foot + bottomCenter) * 0.5;
                ellipse.B(Math::CalcDistanceFormPointToPoint(curPos, bottomCenter) * 0.5);
            }
        }
        break;
    case Mark::Right:
        {
            if (_CanRotate)
            {
                const auto& center = markPositions[static_cast<int>(Mark::Center)];
                auto angle = std::atan2(curPos.Y() - center.Y(), curPos.X() - center.X());
                GetEllipse().Angle(angle);
            }
            else
            {
                const auto& leftCenter = markPositions[static_cast<int>(Mark::Left)];
                const auto& rightCenter = markPositions[static_cast<int>(Mark::Right)];
                double a, b, c;
                if (Math::CalcLineByTwoPoints(rightCenter, leftCenter, a, b, c))
                {
                    Point foot;
                    Math::CalcVerticalFootOfPointToline(curPos, a, b, c, foot);
                    auto& ellipse = GetEllipse();
                    ellipse.Center() = (foot + leftCenter) * 0.5;
                    ellipse.A(Math::CalcDistanceFormPointToPoint(curPos, leftCenter) * 0.5);
                }
            }
        }
        break;
    case Mark::Bottom:
        {
            const auto& topCenter = markPositions[static_cast<int>(Mark::Top)];
            const auto& bottomCenter = markPositions[static_cast<int>(Mark::Bottom)];
            double a, b, c;
            if (Math::CalcLineByTwoPoints(topCenter, bottomCenter, a, b, c))
            {
                Point foot;
                Math::CalcVerticalFootOfPointToline(curPos, a, b, c, foot);
                auto& ellipse = GetEllipse();
                ellipse.Center() = (foot + topCenter) * 0.5;
                ellipse.B(Math::CalcDistanceFormPointToPoint(curPos, topCenter) * 0.5);
            }
        }
        break;
    default:
        break;
    }
}

bool ROIEllipse::Draw()
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
            else if (i == (int)Mark::Right)
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
