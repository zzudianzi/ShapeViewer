#include "pch.h"
#include "VisArc.h"
#include "Display.h"

#include <numbers>

using namespace ShapeViewer;

VisArc::VisArc(const ::ShapeViewer::Arc& arc) : _Arc(arc)
{

}

VisArc::VisArc(const VisArc& obj) : Vis(obj), _Arc(obj._Arc)
{
}

::ShapeViewer::Arc& VisArc::Arc()
{
    return _Arc;
}

const ::ShapeViewer::Arc& VisArc::Arc() const
{
    return _Arc;
}

bool VisArc::CreateD2DFigure()
{
    _D2D1Gemoetry = nullptr;

    assert(_Display);
    if (!_Display)
    {
        return false;
    }

    auto factory = _Display->D2D1Factory();
    auto transform = _Display->TransformToWindow();

    if (!factory)
    {
        return false;
    }

    winrt::check_hresult(factory->CreatePathGeometry(_D2D1Gemoetry.put()));

    winrt::com_ptr<ID2D1GeometrySink> sink;
    winrt::check_hresult(_D2D1Gemoetry->Open(sink.put()));

    auto startPoint = transform.TransformPoint(D2D1::Point2F(_Arc.StartPoint().X(), _Arc.StartPoint().Y()));
    sink->BeginFigure(startPoint, D2D1_FIGURE_BEGIN_FILLED);

    auto scale = _Display->GetSceneScale();
    auto radius = _Arc.Radius() * scale;
    auto rotationAngle = _Arc.RotationRadian() * 180. / std::numbers::pi;

    auto stopPoint = transform.TransformPoint(D2D1::Point2F(_Arc.StopPoint().X(), _Arc.StopPoint().Y()));
    auto arc = D2D1::ArcSegment(
        stopPoint,
        D2D1::SizeF(radius, radius),
        rotationAngle,
        D2D1_SWEEP_DIRECTION_CLOCKWISE,
        _Arc.LargerThanSemiCicle() ? D2D1_ARC_SIZE_LARGE : D2D1_ARC_SIZE_SMALL);

    sink->AddArc(&arc);

    sink->EndFigure(D2D1_FIGURE_END_OPEN);
    winrt::check_hresult(sink->Close());

    return true;
}

void VisArc::Draw()
{
    if (!CreateD2DFigure())
    {
        return;
    }

    auto brush = _Display->Brush();
    brush->SetColor(_Color);
    auto rt = _Display->RenderTarget();
    rt->DrawGeometry(_D2D1Gemoetry.get(), brush.get());
}

std::optional<::ShapeViewer::Rect> VisArc::BoundingRect() const
{
    throw;
}
