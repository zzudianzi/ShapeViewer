#pragma once

#include "Geometry.h"

#include "Point.h"

namespace ShapeViewer
{
class Ellipse : public Geometry
{
  public:
    Ellipse(const ShapeViewer::Point& center, double a, double b, double angle);
    Ellipse(const Ellipse& obj);
    ~Ellipse() override;

    Ellipse& operator=(const Ellipse& obj);
    bool operator==(const Ellipse& obj) const;

    ShapeViewer::Point& Center();
    const ShapeViewer::Point& Center() const;

    double A() const;
    void A(double value);

    double B() const;
    void B(double value);

    bool PtInEllipse(const ShapeViewer::Point& pt);

    bool Equal(const Geometry& obj) const override;
    bool Copy(const Geometry& obj) override;
    Geometry* Clone() const override;

  private:
    ShapeViewer::Point _Center;
    double _A;
    double _B;
    double _Angle;
};
} // namespace ShapeViewer
