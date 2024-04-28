#pragma once

#include "Vis.h"
#include "Polygon.h"

#include <d2d1_3.h>
#include <winrt/base.h>

namespace ShapeViewer
{
class VisPolygon : public Vis
{
  public:
    explicit VisPolygon(const ShapeViewer::Polygon& polygon);

    ShapeViewer::Polygon& Polygon();
    const ShapeViewer::Polygon& Polygon() const;

    bool CreateD2DFigure(ID2D1Factory3* factory, const D2D1::Matrix3x2F& transform) override;
    void Draw(ID2D1RenderTarget* rt, ID2D1SolidColorBrush* brush) override;

    std::optional<::ShapeViewer::Rect> BoundingRect() const override;

  private:
    
    ShapeViewer::Polygon _Polygon;
    winrt::com_ptr<ID2D1PathGeometry> _D2D1Gemoetry;
};
} // namespace ShapeViewer