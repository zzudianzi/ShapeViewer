#include "pch.h"
#include "VisPolygon.h"
#include <ranges>

using namespace ShapeViewer;

VisPolygon::VisPolygon(const ShapeViewer::Polygon& polygon)
{
    _Polygon = polygon;
}

ShapeViewer::Polygon& VisPolygon::Polygon()
{
    return _Polygon;
}

const ShapeViewer::Polygon& VisPolygon::Polygon() const
{
    return _Polygon;
}

bool VisPolygon::CreateD2DFigure(ID2D1Factory3* factory, const D2D1::Matrix3x2F& transform)
{
    _D2D1Gemoetry = nullptr;

    if (!factory)
    {
        return false;
    }

    const auto& vertices = _Polygon.GetVertices();
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

    sink->EndFigure(D2D1_FIGURE_END_CLOSED);
    winrt::check_hresult(sink->Close());

    return true;
}

void VisPolygon::Draw(ID2D1RenderTarget* rt, ID2D1SolidColorBrush* brush)
{
    brush->SetColor(_Color);
    rt->DrawGeometry(_D2D1Gemoetry.get(), brush);
}

std::optional<::ShapeViewer::Rect> VisPolygon::BoundingRect() const
{
    if (!_Visible)
    {
        return {};
    }

    const auto& vertices = Polygon().GetVertices();
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
