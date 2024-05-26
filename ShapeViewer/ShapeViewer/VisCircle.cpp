#include "pch.h"
#include "VisCircle.h"
#include "Display.h"

using namespace ShapeViewer;

VisCircle::VisCircle(const ::ShapeViewer::Circle& circle) : _Circle(circle)
{
}

VisCircle::VisCircle(const ::ShapeViewer::VisCircle& visCircle) : Vis(visCircle), _Circle(visCircle.Circle())
{
}

::ShapeViewer::Circle& VisCircle::Circle()
{
    return _Circle;
}

const ::ShapeViewer::Circle& VisCircle::Circle() const
{
    return _Circle;
}

void VisCircle::Draw()
{
    assert(_Display);
    if (!_Display)
    {
        return;
    }

    auto transform = _Display->TransformToWindow();
    auto center = transform.TransformPoint(D2D1::Point2F(_Circle.Center().X(), _Circle.Center().Y()));
    auto scale = _Display->GetWindowScale();
    auto brush = _Display->Brush();
    brush->SetColor(_Color);

    auto rt = _Display->RenderTarget();
    rt->DrawEllipse(D2D1::Ellipse(center, _Circle.Radius() * scale, _Circle.Radius() * scale), brush.get());
}

std::optional<::ShapeViewer::Rect> VisCircle::BoundingRect() const
{
    return std::make_optional(Rect(_Circle.Center(), _Circle.Radius(), _Circle.Radius()));
}
