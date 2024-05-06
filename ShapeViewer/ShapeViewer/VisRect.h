#pragma once

#include "Rect.h"
#include "Vis.h"

namespace ShapeViewer
{
class VisRect : public ::ShapeViewer::Vis
{
  public:
    explicit VisRect(const ::ShapeViewer::Rect& rect);

    ::ShapeViewer::Rect& Rect();
    const ::ShapeViewer::Rect& Rect() const;

    bool CreateD2DFigure();
    void Draw() override;

    std::optional<::ShapeViewer::Rect> BoundingRect() const override;

  private:
    ::ShapeViewer::Rect _Rect;
    winrt::com_ptr<ID2D1PathGeometry> _D2D1Gemoetry;
};
} // namespace ShapeViewer
