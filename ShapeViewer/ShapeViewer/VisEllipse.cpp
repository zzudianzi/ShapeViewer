#include "pch.h"
#include "VisEllipse.h"
#include "Display.h"
#include "MathTool.h"

using namespace ShapeViewer;

VisEllipse::VisEllipse(const ::ShapeViewer::Ellipse& ellipse) : _Ellipse(ellipse)
{
}

VisEllipse::VisEllipse(const ::ShapeViewer::VisEllipse& visEllipse) : Vis(visEllipse), _Ellipse(visEllipse.Ellipse())
{
}

::ShapeViewer::Ellipse& VisEllipse::Ellipse()
{
    return _Ellipse;
}

const ::ShapeViewer::Ellipse& VisEllipse::Ellipse() const
{
    return _Ellipse;
}

void VisEllipse::Draw()
{
    assert(_Display);
    if (!_Display)
    {
        return;
    }

    auto transform = _Display->TransformToWindow();
    auto center = transform.TransformPoint(D2D1::Point2F(_Ellipse.Center().X(), _Ellipse.Center().Y()));
    auto scale = _Display->GetWindowScale();
    auto brush = _Display->Brush();
    brush->SetColor(_Color);

    auto rt = _Display->RenderTarget();

    auto angle = Math::RadianToAngle(Ellipse().Angle());
    auto matrix = D2D1::Matrix3x2F::Rotation(angle, center);
    D2D1::Matrix3x2F oriTransform;
    rt->GetTransform(&oriTransform);
    rt->SetTransform(oriTransform * matrix);
    rt->DrawEllipse(D2D1::Ellipse(center, _Ellipse.A() * scale, _Ellipse.B() * scale), brush.get());
    rt->SetTransform(oriTransform);
}

std::optional<::ShapeViewer::Rect> VisEllipse::BoundingRect() const
{
    return std::make_optional(Rect(_Ellipse.Center(), _Ellipse.A() * 2., _Ellipse.B() * 2., _Ellipse.Angle()));
}
