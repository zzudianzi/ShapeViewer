#pragma once

#include "Point.h"

namespace ShapeViewer
{
class Line : public Geometry
{
  public:
    Line(const ::ShapeViewer::Point& st, const ::ShapeViewer::Point& ed);
    Line(double a, double b, double c);
    Line(const Line& obj);
    Line& operator=(const Line& obj);

    bool operator==(const Line& obj) const;

    double A() const;
    void A(double value);
    double B() const;
    void B(double value);
    double C() const;
    void C(double value);

    bool Equal(const Geometry& obj) const override;
    bool Copy(const Geometry& obj) override;
    Line* Clone() const override;

  private:
    double _A;
    double _B;
    double _C;
};
} // namespace ShapeViewer
