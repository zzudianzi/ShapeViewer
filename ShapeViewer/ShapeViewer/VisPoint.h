#pragma once

#include "Vis.h"
#include "Point.h"

namespace ShapeViewer
{
class VisPoint : public Vis
{
  public:
    explicit VisPoint(const ::ShapeViewer::Point& point);
    explicit VisPoint(const ::ShapeViewer::VisPoint& visPoint);

    ::ShapeViewer::Point& Point();
    const ::ShapeViewer::Point& Point() const;

    void Draw() override;

    std::optional<::ShapeViewer::Rect> BoundingRect() const override;

  private:
    ::ShapeViewer::Point _Point;
    winrt::com_ptr<ID2D1PathGeometry> _D2D1Geometry;
};
} // namespace ShapeViewer
