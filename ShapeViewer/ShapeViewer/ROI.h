#pragma once

#include "Rect.h"
#include <vector>
#include "Vis.h"

#include <d2d1_3.h>

namespace ShapeViewer
{
class Display;
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

    virtual Point Center() const = 0;
    virtual bool SamePos(const ROI& roi) const = 0;
    virtual void MarkPositions(std::vector<Point>& markPositions) const = 0;
    virtual std::vector<Point> MarkPositions() const = 0;
    virtual bool PtInShape(const Point& pt) const = 0;
    virtual bool PtNearBoundary(const Point& pt, double maxDis) const = 0;
    virtual void DragMark(int selectedMark, const Point& oriPos, const Point& curPos, const ROI& oriROI) = 0;

    virtual const Vis& GetVis() const = 0;
    virtual Vis& GetVis() = 0;

    const Display* GetDisplay() const
    {
        return _Display;
    }
    Display* GetDisplay()
    {
        return _Display;
    }
    void SetDisplay(Display* display)
    {
        _Display = display;
    }

    static constexpr double MarkRadius = 3.;

    virtual bool Draw(ID2D1RenderTarget* rt, ID2D1SolidColorBrush* brush, const D2D1::Matrix3x2F& transform) = 0;

  protected:
    ROI();

    bool _DrawMark;
    bool _Visible;
    bool _Scaling;

    Display* _Display;

    D2D1_COLOR_F _MarkColor;
};
} // namespace ShapeViewer