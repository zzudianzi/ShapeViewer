#pragma once
#include "ROI.h"
#include "VisPolygon.h"

namespace ShapeViewer
{
class ROIPolygon : public ROI
{
  public:
    enum class Mark
    {
        Center
    };

    ROI* Clone() const override;

    explicit ROIPolygon(const Polygon& polygon);
    explicit ROIPolygon(const VisPolygon& visPolygon);
    explicit ROIPolygon(const ROIPolygon& roiPolygon);

    ::ShapeViewer::Polygon& GetPolygon();
    const ::ShapeViewer::Polygon& GetPolygon() const;

    ::ShapeViewer::VisPolygon& GetVisPolygon();
    const ::ShapeViewer::VisPolygon& GetVisPolygon() const;

    const Vis& GetVis() const override;
    Vis& GetVis() override;

    Point Center() const override;
    bool SamePos(const ROI& roi) const override;
    void MarkPositions(std::vector<Point>& markPositions) const override;
    std::vector<Point> MarkPositions() const override;
    bool PtInShape(const Point& pt) const override;
    bool PtNearBoundary(const Point& pt, double maxDis) const override;
    void DragMark(int selectedMark, const Point& oriPos, const Point& curPos, const ROI& oriROI) override;

    bool Draw(ID2D1RenderTarget* rt, ID2D1SolidColorBrush* brush, const D2D1::Matrix3x2F& transform) override;

  private:
    ::ShapeViewer::VisPolygon _VisPolygon;
};
} // namespace ShapeViewer
