#include "pch.h"
#include "ROIRect.h"
#include "MathTool.h"
#include "Display.h"

#include <d2d1helper.h>

using namespace ShapeViewer;

ROI* ROIRect::Clone() const
{
    return new ROIRect(*this);
}

ROIRect::ROIRect(const ::ShapeViewer::Rect& rect) : _VisRect(rect), _CanRotate(false)
{
}

ROIRect::ROIRect(const ::ShapeViewer::VisRect& visRect) : _VisRect(visRect), _CanRotate(false)
{
}

ROIRect::ROIRect(const ::ShapeViewer::ROIRect& roiRect) : _VisRect(roiRect.GetVisRect()), _CanRotate(false)
{
}

::ShapeViewer::Rect& ROIRect::GetRect()
{
    return GetVisRect().Rect();
}

const ::ShapeViewer::Rect& ROIRect::GetRect() const
{
    return GetVisRect().Rect();
}

::ShapeViewer::VisRect& ROIRect::GetVisRect()
{
    return _VisRect;
}

const ::ShapeViewer::VisRect& ROIRect::GetVisRect() const
{
    return _VisRect;
}

::ShapeViewer::Vis& ROIRect::GetVis()
{
    return _VisRect;
}

const ::ShapeViewer::Vis& ROIRect::GetVis() const
{
    return _VisRect;
}

::ShapeViewer::Point ROIRect::Center() const
{
    return GetVisRect().Rect().Center();
}

bool ROIRect::CanRotate() const
{
    return _CanRotate;
}

void ROIRect::CanRotate(bool value)
{
    _CanRotate = value;
}

bool ROIRect::SamePos(const ROI& roi) const
{
    auto rect = dynamic_cast<const ROIRect*>(&roi);
    if (rect == nullptr)
    {
        return false;
    }

    if (rect == this)
    {
        return true;
    }

    return GetRect() == rect->GetRect();
}

void ROIRect::MarkPositions(std::vector<Point>& markPositions) const
{
    markPositions.clear();
    markPositions.resize((int)Mark::Count);
    auto corners = GetRect().Corners();

    markPositions[static_cast<int>(Mark::Center)] = GetRect().Center();
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
    markPositions[static_cast<int>(Mark::TopLeft)] = corners[static_cast<int>(Rect::Corner::TopLeft)];
    markPositions[static_cast<int>(Mark::TopRight)] = corners[static_cast<int>(Rect::Corner::TopRight)];
    markPositions[static_cast<int>(Mark::BottomRight)] = corners[static_cast<int>(Rect::Corner::BottomRight)];
    markPositions[static_cast<int>(Mark::BottomLeft)] = corners[static_cast<int>(Rect::Corner::BottomLeft)];
}

std::vector<Point> ROIRect::MarkPositions() const
{
    std::vector<Point> markPositions;
    MarkPositions(markPositions);
    return markPositions;
}

bool ROIRect::PtInShape(const Point& pt) const
{
    return Math::PtInPolygon(pt, GetRect().Corners());
}

bool ROIRect::PtNearBoundary(const Point& pt, double maxDis) const
{
    return Math::PointNearBoundary(pt, GetRect().Corners(), maxDis);
}

void ROIRect::DragMark(int selectedMark, const Point& oriPos, const Point& curPos, const ROI& oriROI)
{
    auto oriROIRect = dynamic_cast<const ROIRect*>(&oriROI);
    auto markPositions = oriROIRect->MarkPositions();
    switch ((Mark)selectedMark)
    {
    case Mark::TopLeft:
        {
            GetRect().St() = curPos;
            UpdateAngle();
        }
        break;
    case Mark::Center:
        {
            auto offset = curPos - oriPos;
            const auto& oriRect = oriROIRect->GetRect();
            GetRect().St() = oriRect.St() + offset;
            GetRect().Ed() = oriRect.Ed() + offset;
        }
        break;
    case Mark::Left:
        {
            const auto& leftCenter = markPositions[static_cast<int>(Mark::Left)];
            const auto& rightCenter = markPositions[static_cast<int>(Mark::Right)];
            double a, b, c;
            if (Math::CalcLineByTwoPoints(leftCenter, rightCenter, a, b, c))
            {
                Point foot;
                Math::CalcVerticalFootOfPointToline(curPos, a, b, c, foot);
                auto offset = foot - leftCenter;
                const auto& oriRect = oriROIRect->GetRect();
                GetRect().St() = oriRect.St() + offset;
                UpdateAngle();
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
                auto offset = foot - topCenter;
                const auto& oriRect = oriROIRect->GetRect();
                GetRect().St() = oriRect.St() + offset;
                UpdateAngle();
            }
        }
        break;
    case Mark::Right:
        {
            if (_CanRotate)
            {
                const auto& center = markPositions[static_cast<int>(Mark::Center)];
                auto angle = std::atan2(curPos.Y() - center.Y(), curPos.X() - center.X());
                GetRect().Angle(angle);
                auto angleRotate = GetRect().Angle() - oriROIRect->GetRect().Angle();

                D2D1::Matrix3x2F mat = D2D1::Matrix3x2F::Rotation(angleRotate * 180. / DirectX::XM_PI, D2D1::Point2F(center.X(), center.Y()));

                const auto& topLeft = markPositions[static_cast<int>(Mark::TopLeft)];
                const auto& bottomRight = markPositions[static_cast<int>(Mark::BottomRight)];

                auto ptTopLeft = mat.TransformPoint(D2D1::Point2F(topLeft.X(), topLeft.Y()));
                auto ptBottomRight = mat.TransformPoint(D2D1::Point2F(bottomRight.X(), bottomRight.Y()));

                GetRect().St() = Point(ptTopLeft.x, ptTopLeft.y);
                GetRect().Ed() = Point(ptBottomRight.x, ptBottomRight.y);
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
                    auto offset = foot - rightCenter;
                    const auto& oriRect = oriROIRect->GetRect();
                    GetRect().Ed() = oriRect.Ed() + offset;
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
                auto offset = foot - bottomCenter;
                const auto& oriRect = oriROIRect->GetRect();
                GetRect().Ed() = oriRect.Ed() + offset;
                UpdateAngle();
            }
        }
        break;
    case Mark::TopRight:
        {
            const auto& topLeft = markPositions[static_cast<int>(Mark::TopLeft)];
            const auto& topRight = markPositions[static_cast<int>(Mark::TopRight)];
            const auto& bottomRight = markPositions[static_cast<int>(Mark::BottomRight)];
            double a1, b1, c1, a2, b2, c2;
            if (Math::CalcLineByTwoPoints(topRight, bottomRight, a1, b1, c1) &&
                Math::CalcLineByTwoPoints(topLeft, topRight, a2, b2, c2))
            {
                Point foot1, foot2;
                Math::CalcVerticalFootOfPointToline(curPos, a1, b1, c1, foot1);
                auto offset1 = foot1 - topRight;
                Math::CalcVerticalFootOfPointToline(curPos, a2, b2, c2, foot2);
                auto offset2 = foot2 - topRight;
                const auto& oriRect = oriROIRect->GetRect();
                GetRect().St() = oriRect.St() + offset1;
                GetRect().Ed() = oriRect.Ed() + offset2;
                UpdateAngle();
            }
        }
        break;
    case Mark::BottomRight:
        {
            GetRect().Ed() = curPos;
            UpdateAngle();
        }
        break;
    case Mark::BottomLeft:
        {
            const auto& topLeft = markPositions[static_cast<int>(Mark::TopLeft)];
            const auto& bottomLeft = markPositions[static_cast<int>(Mark::BottomLeft)];
            const auto& bottomRight = markPositions[static_cast<int>(Mark::BottomRight)];
            double a1, b1, c1, a2, b2, c2;
            if (Math::CalcLineByTwoPoints(topLeft, bottomLeft, a1, b1, c1) &&
                Math::CalcLineByTwoPoints(bottomLeft, bottomRight, a2, b2, c2))
            {
                Point foot1, foot2;
                Math::CalcVerticalFootOfPointToline(curPos, a1, b1, c1, foot1);
                auto offset1 = foot1 - bottomLeft;
                Math::CalcVerticalFootOfPointToline(curPos, a2, b2, c2, foot2);
                auto offset2 = foot2 - bottomLeft;
                const auto& oriRect = oriROIRect->GetRect();
                GetRect().St() = oriRect.St() + offset2;
                GetRect().Ed() = oriRect.Ed() + offset1;

                UpdateAngle();
            }
        }
        break;
    default:
        break;
    }
}

bool ROIRect::Draw()
{
    auto display = GetVis().GetDisplay();
    auto brush = display->Brush();
    auto rt = display->RenderTarget();
    auto transform = display->TransformToWindow();
    GetVis().Draw();
    if (_DrawMark)
    {
        auto markPositions = MarkPositions();
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

void ROIRect::UpdateAngle()
{
    auto markPositions = MarkPositions();
    const auto& leftCenter = markPositions[static_cast<int>(Mark::Left)];
    const auto& rightCenter = markPositions[static_cast<int>(Mark::Right)];
    auto angle = std::atan2(rightCenter.Y() - leftCenter.Y(), rightCenter.X() - leftCenter.X());
    GetRect().Angle(angle);
}
