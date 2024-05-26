#pragma once

#include "ROI.h"
#include "VisCircle.h"

namespace ShapeViewer
{
class ROICircle : public ::ShapeViewer::ROI
{
  public:
    explicit ROICircle(const ::ShapeViewer::Circle& circle);
    explicit ROICircle(const ::ShapeViewer::ROICircle& roi);

    enum class Mark
    {
        Center = 0,
        Left,
        Top,
        Right,
        Bottom,
        Count
    };

    ::ShapeViewer::Circle& GetCircle();
    const ::ShapeViewer::Circle& GetCircle() const;

    ::ShapeViewer::VisCircle& GetVisCircle();
    const ::ShapeViewer::VisCircle& GetVisCircle() const;

    ::ShapeViewer::Vis& GetVis() override;
    const ::ShapeViewer::Vis& GetVis() const override;

    ROI* Clone() const override;
    ::ShapeViewer::Point Center() const override;
    bool SamePos(const ::ShapeViewer::ROI& roi) const override;
    void MarkPositions(std::vector<::ShapeViewer::Point>& markPositions) const override;
    bool PtInShape(const ::ShapeViewer::Point& pt) const override;
    bool PtNearBoundary(const ::ShapeViewer::Point& pt, double maxDis) const override;
    void DragMark(
        int selectedMark,
        const ::ShapeViewer::Point& oriPos,
        const ::ShapeViewer::Point& curPos,
        const ::ShapeViewer::ROI& oriROI) override;

    bool Draw() override;

  private:
    ::ShapeViewer::VisCircle _VisCircle;
};
} // namespace ShapeViewer
