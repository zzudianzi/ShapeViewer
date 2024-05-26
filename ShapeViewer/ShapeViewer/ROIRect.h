#pragma once

#include "ROI.h"
#include "VisRect.h"

namespace ShapeViewer
{
class ROIRect : public ::ShapeViewer::ROI
{
  public:
    enum class Mark
    {
        Center,
        Left,
        Top,
        Right,
        Bottom,
        TopLeft,
        TopRight,
        BottomRight,
        BottomLeft,
        Count
    };

    ROI* Clone() const override;

    explicit ROIRect(const ::ShapeViewer::Rect& rect);
    explicit ROIRect(const ::ShapeViewer::VisRect& visRect);
    explicit ROIRect(const ::ShapeViewer::ROIRect& roiRect);

    ::ShapeViewer::Rect& GetRect();
    const ::ShapeViewer::Rect& GetRect() const;

    ::ShapeViewer::VisRect& GetVisRect();
    const ::ShapeViewer::VisRect& GetVisRect() const;

    ::ShapeViewer::Vis& GetVis() override;
    const ::ShapeViewer::Vis& GetVis() const override;

    ::ShapeViewer::Point Center() const override;

    bool CanRotate() const;
    void CanRotate(bool value);

    bool SamePos(const ROI& roi) const override;
    void MarkPositions(std::vector<Point>& markPositions) const override;
    bool PtInShape(const Point& pt) const override;
    bool PtNearBoundary(const Point& pt, double maxDis) const override;
    void DragMark(int selectedMark, const Point& oriPos, const Point& curPos, const ROI& oriROI) override;

    bool Draw() override;

  private:
    void UpdateAngle();

  private:
    ::ShapeViewer::VisRect _VisRect;
    bool _CanRotate;
};
} // namespace ShapeViewer
