#pragma once

#include "Vis.h"
#include "Circle.h"

namespace ShapeViewer
{
class VisCircle : public ::ShapeViewer::Vis
{
  public:
    explicit VisCircle(const ::ShapeViewer::Circle& circle);
    explicit VisCircle(const ::ShapeViewer::VisCircle& visCircle);

    ::ShapeViewer::Circle& Circle();
    const ::ShapeViewer::Circle& Circle() const;

    void Draw() override;

    std::optional<::ShapeViewer::Rect> BoundingRect() const override;

  private:
    ::ShapeViewer::Circle _Circle;
};
} // namespace ShapeViewer
