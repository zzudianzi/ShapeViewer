#pragma once

#include "Vis.h"
#include "Ellipse.h"

namespace ShapeViewer
{
class VisEllipse : public ::ShapeViewer::Vis
{
  public:
    explicit VisEllipse(const ::ShapeViewer::Ellipse& Ellipse);
    explicit VisEllipse(const ::ShapeViewer::VisEllipse& visEllipse);

    ::ShapeViewer::Ellipse& Ellipse();
    const ::ShapeViewer::Ellipse& Ellipse() const;

    void Draw() override;

    std::optional<::ShapeViewer::Rect> BoundingRect() const override;

  private:
    ::ShapeViewer::Ellipse _Ellipse;
};
} // namespace ShapeViewer
