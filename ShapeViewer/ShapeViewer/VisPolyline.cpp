#include "pch.h"
#include "VisPolyline.h"
#include "Display.h"

using namespace ShapeViewer;

VisPolyline::VisPolyline(const ShapeViewer::Polyline& Polyline)
{
    _Polyline = Polyline;
}

ShapeViewer::Polyline& VisPolyline::Polyline()
{
    return _Polyline;
}

const ShapeViewer::Polyline& VisPolyline::Polyline() const
{
    return _Polyline;
}

bool VisPolyline::CreateD2DFigure()
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

    const auto& vertices = _Polyline.GetVertices();
    if (vertices.empty())
    {
        return false;
    }

    winrt::check_hresult(factory->CreatePathGeometry(_D2D1Gemoetry.put()));

    winrt::com_ptr<ID2D1GeometrySink> sink;
    winrt::check_hresult(_D2D1Gemoetry->Open(sink.put()));

    auto pt1 = transform.TransformPoint(D2D1::Point2F(vertices[0].X(), vertices[0].Y()));
    sink->BeginFigure(
        transform.TransformPoint(D2D1::Point2F(vertices[0].X(), vertices[0].Y())), D2D1_FIGURE_BEGIN_FILLED);

    for (const auto& vertex : std::ranges::subrange(vertices.begin() + 1, vertices.end()))
    {
        auto pt2 = transform.TransformPoint(D2D1::Point2F(vertex.X(), vertex.Y()));
        sink->AddLine(transform.TransformPoint(D2D1::Point2F(vertex.X(), vertex.Y())));
    }

    sink->EndFigure(D2D1_FIGURE_END_OPEN);
    winrt::check_hresult(sink->Close());

    return true;
}

void VisPolyline::Draw()
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

std::optional<::ShapeViewer::Rect> VisPolyline::BoundingRect() const
{
    if (!_Visible)
    {
        return {};
    }

    const auto& vertices = Polyline().GetVertices();
    assert(!vertices.empty());
    if (vertices.empty())
    {
        return {};
    }

    auto rc =
        std::make_optional<::ShapeViewer::Rect>(vertices[0].X(), vertices[0].Y(), vertices[0].X(), vertices[0].Y());

    for (int i = 1; i < (int)vertices.size(); i++)
    {
        if (vertices[i].X() < rc->St().X())
        {
            rc->St().X(vertices[i].X());
        }
        if (vertices[i].Y() < rc->St().Y())
        {
            rc->St().Y(vertices[i].Y());
        }
        if (vertices[i].X() > rc->Ed().X())
        {
            rc->Ed().X(vertices[i].X());
        }
        if (vertices[i].Y() > rc->Ed().Y())
        {
            rc->Ed().Y(vertices[i].Y());
        }
    }

    return rc;
}
