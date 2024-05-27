#pragma once

#include "Vis.h"
#include "Arc.h"

namespace ShapeViewer
{
class VisArc : public Vis
{
  public:
    explicit VisArc(const ::ShapeViewer::Arc& arc);
    explicit VisArc(const VisArc& obj);

    ::ShapeViewer::Arc& Arc();
    const ::ShapeViewer::Arc& Arc() const;

    bool CreateD2DFigure();
    void Draw() override;

    std::optional<::ShapeViewer::Rect> BoundingRect() const override;

  private:
    ::ShapeViewer::Arc _Arc;
    winrt::com_ptr<ID2D1PathGeometry> _D2D1Gemoetry;
};
} // namespace ShapeViewer
