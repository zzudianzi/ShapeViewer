#pragma once

#include "Point.h"

namespace ShapeViewer
{
class Arc : public Geometry
{
  public:
    ::ShapeViewer::Arc(const ::ShapeViewer::Point& center, double radius, double startRadian, double stopRadian);
    ::ShapeViewer::Arc(const ::ShapeViewer::Arc& obj);
    ::ShapeViewer::Arc& operator=(const ::ShapeViewer::Arc& obj);

    bool operator==(const ::ShapeViewer::Arc& obj) const;

    ::ShapeViewer::Point& Center();
    const Point& Center() const;

    ::ShapeViewer::Point PointAtRadian(double radian) const;

    double Radius() const;
    void Radius(double value);

    ::ShapeViewer::Point StartPoint() const;
    ::ShapeViewer::Point StopPoint() const;

    double RotationRadian() const;

    bool LargerThanSemiCicle() const;

    double StartRadian() const;
    void StartRadian(double value);

    double RestrictStartRadian() const;
    double RestrictStopRadian() const;
    static double RestrictRadian(double radian);

    double StopRadian() const;
    void StopRadian(double value);

    bool Equal(const ::ShapeViewer::Geometry& obj) const override;
    bool Copy(const ::ShapeViewer::Geometry& obj) override;
    ::ShapeViewer::Arc* Clone() const override;

  private:
    Point _Center;
    double _Radius;
    double _StartRadian;
    double _StopRadian;
};
} // namespace ShapeViewer
