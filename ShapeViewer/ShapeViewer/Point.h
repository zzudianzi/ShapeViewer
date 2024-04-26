#pragma once

#include "Geometry.h"

namespace ShapeViewer
{
class Point : public Geometry
{
  public:
    Point();
    Point(double x, double y);
    Point(const Point& obj);

    double X() const;
    void X(double value);

    double Y() const;
    void Y(double value);

    Point& operator=(const Point& obj);
    bool operator==(const Geometry& obj) const override;

    Point operator*(double value) const;
    Point operator+(const Point& obj) const;
    Point operator-(const Point& obj) const;

    bool Copy(const Geometry& obj) override;
    Point* Clone() const override;

  private:
    double _X;
    double _Y;
};

bool operator==(const Point& l, const Point& r);

} // namespace ShapeViewer
