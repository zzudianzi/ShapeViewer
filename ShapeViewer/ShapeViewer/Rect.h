#pragma once

#include "Geometry.h"
#include "Point.h"

namespace ShapeViewer
{
class Rect : public Geometry
{
  public:
    Rect();
    Rect(double xs, double ys, double xe, double ye, double angle = 0.);
    Rect(Point center, double width, double height, double angle = 0.);
    Rect(Point st, Point ed, double angle = 0.);
    explicit Rect(const Rect& obj);

    enum class Corner
    {
        TopLeft,
        TopRight,
        BottomRight,
        BottomLeft,
    };

    Point& St();
    const Point& St() const;

    Point& Ed();
    const Point& Ed() const;

    Point Center() const;

    double Angle() const;
    void Angle(double value);

    std::vector<Point> Corners() const;
    void Corners(std::vector<Point>& corners) const;

    double Width() const;
    double Height() const;

    bool PtInRect(const Point& pt) const;
    void SetRectByCorners(
        const Point& topLeft, const Point& topRight, const Point& bottomRight, const Point& bottomLeft);

    bool operator==(const Geometry& obj) const override;
    bool Copy(const Geometry& obj) override;
    Rect* Clone() const override;

    Rect operator|(const Rect& rc);

    Rect& operator|=(const Rect& rc);

  private:
    Point _St;
    Point _Ed;
    double _Angle;
};

bool operator==(const Rect& l, const Rect& r);
} // namespace ShapeViewer
