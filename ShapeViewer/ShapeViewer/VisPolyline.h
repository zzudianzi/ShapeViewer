#pragma once

#include "Polyline.h"
#include "Vis.h"

namespace ShapeViewer
{
class VisPolyline : public ::ShapeViewer::Vis
{
  public:
    explicit VisPolyline(const ShapeViewer::Polyline& Polyline);

    ShapeViewer::Polyline& Polyline();
    const ShapeViewer::Polyline& Polyline() const;

    bool CreateD2DFigure();
    void Draw() override;

    std::optional<::ShapeViewer::Rect> BoundingRect() const override;

  private:
    ShapeViewer::Polyline _Polyline;
    winrt::com_ptr<ID2D1PathGeometry> _D2D1Gemoetry;
};
}