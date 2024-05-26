#include "pch.h"
#include "VisPoint.h"
#include "Display.h"
#include "Rect.h"

using namespace ShapeViewer;

VisPoint::VisPoint(const ::ShapeViewer::Point& point) : _Point(point)
{
}

VisPoint::VisPoint(const ::ShapeViewer::VisPoint& visPoint) : Vis(visPoint), _Point(visPoint.Point())
{

}

::ShapeViewer::Point& VisPoint::Point()
{
    return _Point;
}

const ::ShapeViewer::Point& VisPoint::Point() const
{
    return _Point;
}

void VisPoint::Draw()
{
    assert(_Display);

    if (!_Display)
    {
        return;
    }

    auto transform = _Display->TransformToWindow();

    auto center = transform.TransformPoint(D2D1::Point2F(_Point.X(), _Point.Y()));

    auto brush = _Display->Brush();
    brush->SetColor(_Color);

    auto rt = _Display->RenderTarget();
    auto len = Vis::CrossHalfLength;
    rt->DrawLine(D2D1::Point2F(center.x - len, center.y - len), D2D1::Point2F(center.x + len, center.y + len), brush.get());
    rt->DrawLine(
        D2D1::Point2F(center.x - len, center.y + len), D2D1::Point2F(center.x + len, center.y - len), brush.get());
}

std::optional<::ShapeViewer::Rect> VisPoint::BoundingRect() const
{
    return std::make_optional(Rect(_Point, 0., 0.));
}
