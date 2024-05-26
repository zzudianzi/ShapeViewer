#pragma once

#include "ROI.h"
#include "VisEllipse.h"

namespace ShapeViewer
{
class ROIEllipse : public ::ShapeViewer::ROI
{
  public:
    explicit ROIEllipse(const ::ShapeViewer::Ellipse& ellipse);
    explicit ROIEllipse(const ::ShapeViewer::ROIEllipse& roi);

    enum class Mark
    {
        Center,
        Left,
        Top,
        Right,
        Bottom,
        Count
    };

    ::ShapeViewer::Ellipse& GetEllipse();
    const ::ShapeViewer::Ellipse& GetEllipse() const;

    ::ShapeViewer::VisEllipse& GetVisEllipse();
    const ::ShapeViewer::VisEllipse& GetVisEllipse() const;

    ::ShapeViewer::Vis& GetVis() override;
    const ::ShapeViewer::Vis& GetVis() const override;

    bool CanRotate() const;
    void CanRotate(bool value);

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
    ::ShapeViewer::VisEllipse _VisEllipse;
    bool _CanRotate;
};
} // namespace ShapeViewer
