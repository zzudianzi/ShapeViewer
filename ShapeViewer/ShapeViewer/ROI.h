#pragma once

#include "Rect.h"
#include <vector>

namespace ShapeViewer
{
class ROI
{
  public:
    virtual ~ROI() = default;
    virtual ROI Clone() const = 0;

    bool DrawMark() const;
    void DrawMark(bool value);

    bool Visible() const;
    void Visible(bool value);

    virtual Point Center() const = 0;
    virtual bool SamePos(const ROI& roi) const = 0;
    virtual void MarkPositions(std::vector<Point>& markPositions) const = 0;
    virtual std::vector<Point> MarkPositions() const = 0;
    virtual bool PtInShape(const Point& pt) const = 0;
    virtual bool PtNearBoundary(const Point& pt, double maxDis) const = 0;
    virtual bool DragMark(int selectedMark, const Point& oriPos, const Point& curPos, const ROI& oriROI) = 0;

    static constexpr int MarkRadius = 3;

  protected:
    ROI();

  private:
    bool _DrawMark;
    bool _Visible;

    friend class FXMDisplay;
};
} // namespace ShapeViewer
