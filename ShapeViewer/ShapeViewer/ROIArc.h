#pragma once

#include "ROI.h"
#include "VisArc.h"

namespace ShapeViewer
{
class ROIArc : public ::ShapeViewer::ROI
{
  public:
    explicit ROIArc(const ::ShapeViewer::Arc& arc);
    explicit ROIArc(const ::ShapeViewer::ROIArc& roi);
    ::ShapeViewer::ROI* Clone() const override;

    enum class Mark
    {
        Start,
        Stop,
        Move,
        Roundness,
        Count
    };

    ::ShapeViewer::Arc& GetArc();
    const ::ShapeViewer::Arc& GetArc() const;

    ::ShapeViewer::VisArc& GetVisArc();
    const ::ShapeViewer::VisArc& GetVisArc() const;

    ::ShapeViewer::Vis& GetVis() override;
    const ::ShapeViewer::Vis& GetVis() const override;

    ::ShapeViewer::Point Center() const override;

    bool SamePos(const ROI& roi) const override;
    void MarkPositions(std::vector<Point>& markPositions) const override;
    bool PtInShape(const Point& pt) const override;
    bool PtNearBoundary(const Point& pt, double maxDis) const override;
    void DragMark(int selectedMark, const Point& oriPos, const Point& curPos, const ROI& oriROI) override;

    bool Draw() override;

    static constexpr double MarkOffset = 10.;

  private:
    ::ShapeViewer::VisArc _VisArc;
};
} // namespace ShapeViewer
