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

bool VisPolygon::CreatePathGeometry(ID2D1Factory3* factory)
{
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
    sink->BeginFigure(D2D1::Point2F(vertices[0].X(), vertices[0].Y()), D2D1_FIGURE_BEGIN_FILLED);

    for (const auto& vertex : std::ranges::subrange(vertices.begin() + 1, vertices.end()))
    {
        sink->AddLine(D2D1::Point2F(vertex.X(), vertex.Y()));
    }

    sink->EndFigure(D2D1_FIGURE_END_CLOSED);
    winrt::check_hresult(sink->Close());

    return true;
}

void VisPolygon::Draw(ID2D1RenderTarget* rt, ID2D1Brush* brush)
{
    rt->DrawGeometry(_D2D1Gemoetry.get(), brush);
}
