#pragma once

#include "Geometry.h"
#include "Point.h"

namespace ShapeViewer
{
class Circle : public Geometry
{
  public:
    Circle(const ShapeViewer::Point& center, double radius);
    Circle(const Circle& obj);
    ~Circle() override;

    Circle& operator=(const Circle& obj);
    bool operator==(const Circle& obj) const;

    Point& Center();
    const Point& Center() const;

    double Radius() const;
    void Radius(double value);

    bool PtInCircle(const ShapeViewer::Point& pt) const;

    bool Equal(const Geometry& obj) const override;
    bool Copy(const Geometry& obj) override;
    Circle* Clone() const override;

  private:
    ShapeViewer::Point _Center;
    double _Radius;
};
} // namespace ShapeViewer
