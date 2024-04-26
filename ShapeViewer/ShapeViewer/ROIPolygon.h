#pragma once
#include "ROI.h"
#include "Polygon.h"

namespace ShapeViewer
{
class ROIPolygon : public ROI
{
  public:
    explicit ROIPolygon(const Polygon& polygon);

    Point Center() const override;
    bool SamePos(const ROI& roi) const override;
    void MarkPositions(std::vector<Point>& markPositions) const override;
    std::vector<Point> MarkPositions() const override;
    bool PtInShape(const Point& pt) const override;
    bool PtNearBoundary(const Point& pt, double maxDis) const override;
    bool DragMark(int selectedMark, const Point& oriPos, const Point& curPos, const ROI& oriROI) override;

  private:
    ShapeViewer::Polygon _Polygon;
};
} // namespace ShapeViewer
