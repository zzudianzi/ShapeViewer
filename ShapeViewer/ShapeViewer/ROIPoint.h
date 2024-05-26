#pragma once

#include "ROI.h"
#include "VisPoint.h"

namespace ShapeViewer
{
class ROIPoint : public ::ShapeViewer::ROI
{
  public:
    enum class Mark
    {
        Center = 0
    };
    explicit ROIPoint(const ::ShapeViewer::Point& point);
    explicit ROIPoint(const ROIPoint& roi);
    ROI* Clone() const override;

    ::ShapeViewer::Point& GetPoint();
    const ::ShapeViewer::Point& GetPoint() const;

    ::ShapeViewer::VisPoint& GetVisPoint();
    const ::ShapeViewer::VisPoint& GetVisPoint() const;

    ::ShapeViewer::Vis& GetVis() override;
    const ::ShapeViewer::Vis& GetVis() const override;

    ::ShapeViewer::Point Center() const override;

    bool SamePos(const ROI& roi) const override;
    void MarkPositions(std::vector<Point>& markPositions) const override;
    std::vector<Point> MarkPositions() const override;
    bool PtInShape(const Point& pt) const override;
    bool PtNearBoundary(const Point& pt, double maxDis) const override;
    void DragMark(int selectedMark, const Point& oriPos, const Point& curPos, const ROI& oriROI) override;

    bool Draw() override;

  private:
    ::ShapeViewer::VisPoint _VisPoint;
};
} // namespace ShapeViewer
