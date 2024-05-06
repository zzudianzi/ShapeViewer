#pragma once
#include "ROI.h"
#include "VisPolyline.h"

namespace ShapeViewer
{
class ROIPolyline : public ROI
{
  public:
    enum class Mark
    {
        Center
    };

    ROI* Clone() const override;

    explicit ROIPolyline(const Polyline& Polyline);
    explicit ROIPolyline(const VisPolyline& visPolyline);
    explicit ROIPolyline(const ROIPolyline& roiPolyline);

    ::ShapeViewer::Polyline& GetPolyline();
    const ::ShapeViewer::Polyline& GetPolyline() const;

    ::ShapeViewer::VisPolyline& GetVisPolyline();
    const ::ShapeViewer::VisPolyline& GetVisPolyline() const;

    const Vis& GetVis() const override;
    Vis& GetVis() override;

    Point Center() const override;
    bool SamePos(const ROI& roi) const override;
    void MarkPositions(std::vector<Point>& markPositions) const override;
    std::vector<Point> MarkPositions() const override;
    bool PtInShape(const Point& pt) const override;
    bool PtNearBoundary(const Point& pt, double maxDis) const override;
    void DragMark(int selectedMark, const Point& oriPos, const Point& curPos, const ROI& oriROI) override;

    bool Draw() override;

  private:
    ::ShapeViewer::VisPolyline _VisPolyline;
};
} // namespace ShapeViewer
