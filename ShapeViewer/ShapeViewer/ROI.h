#pragma once

#include "Rect.h"
#include <vector>
#include "Vis.h"

#include <d2d1_3.h>

namespace ShapeViewer
{
class ROI
{
  public:
    virtual ~ROI() = default;
    virtual ROI* Clone() const = 0;

    bool DrawMark() const;
    void DrawMark(bool value);

    bool Visible() const;
    void Visible(bool value);

    bool Scaling() const;
    void Scaling(bool value);

    D2D1_COLOR_F MarkColor() const;
    void MarkColor(D2D1_COLOR_F value);

    virtual ::ShapeViewer::Point Center() const = 0;
    virtual bool SamePos(const ::ShapeViewer::ROI& roi) const = 0;
    virtual void MarkPositions(std::vector<Point>& markPositions) const = 0;
    std::vector<::ShapeViewer::Point> MarkPositions() const;
    virtual bool PtInShape(const ::ShapeViewer::Point& pt) const = 0;
    virtual bool PtNearBoundary(const ::ShapeViewer::Point& pt, double maxDis) const = 0;
    virtual bool PtNearMark(const ::ShapeViewer::Point& pt, double maxDis) const;
    virtual void DragMark(
        int selectedMark,
        const ::ShapeViewer::Point& oriPos,
        const ::ShapeViewer::Point& curPos,
        const ::ShapeViewer::ROI& oriROI) = 0;

    virtual const Vis& GetVis() const = 0;
    virtual Vis& GetVis() = 0;

    const ::ShapeViewer::Display* GetDisplay() const;
    ::ShapeViewer::Display* GetDisplay();
    void SetDisplay(::ShapeViewer::Display* display);

    static constexpr double MarkRadius = 3.;

    virtual bool Draw() = 0;

  protected:
    ROI();

    bool _DrawMark;
    bool _Visible;
    bool _Scaling;

    D2D1_COLOR_F _MarkColor;
};
} // namespace ShapeViewer
