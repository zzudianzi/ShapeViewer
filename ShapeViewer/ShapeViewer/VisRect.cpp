#include "pch.h"
#include "VisRect.h"
#include "MathTool.h"
#include "Display.h"

using namespace ShapeViewer;

VisRect::VisRect(const ::ShapeViewer::Rect& rect)
{
    _Rect = rect;
}

::ShapeViewer::Rect& VisRect::Rect()
{
    return _Rect;
}

const ::ShapeViewer::Rect& VisRect::Rect() const
{
    return _Rect;
}

bool VisRect::CreateD2DFigure()
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

    const auto& vertices = Rect().Corners();
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

void VisRect::Draw()
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

std::optional<::ShapeViewer::Rect> VisRect::BoundingRect() const
{
    const auto& corners = Rect().Corners();
    assert(corners.size() == 4);
    return Math::CalcBoundingRect(corners);
}
