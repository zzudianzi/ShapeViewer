#include "pch.h"
#include "Arc.h"
#include "MathTool.h"
#include "TemplateHelper.hpp"

#include <numbers>

using namespace ShapeViewer;

namespace
{
auto pi2 = 2. * std::numbers::pi;
auto roundRadian = [](double radian) -> double {
    int k = (int)(radian / pi2);
    radian = radian - k * pi2;

    if (std::abs(radian) < Math::Epsilon)
    {
        return 0;
    }

    if (radian < 0)
    {
        radian += pi2;
    }

    return radian;
};
}

Arc::Arc(const Point& center, double radius, double startRadian, double stopRadian)
    : _Center(center), _Radius(radius), _StartRadian(startRadian), _StopRadian(stopRadian)
{
}

Arc::Arc(const ::ShapeViewer::Arc& obj)
    : _Center(obj._Center), _Radius(obj._Radius), _StartRadian(obj._StartRadian), _StopRadian(obj._StopRadian)
{
}

::ShapeViewer::Arc& Arc::operator=(const ::ShapeViewer::Arc& obj)
{
    _Center = obj._Center;
    _Radius = obj._Radius;
    _StartRadian = obj._StartRadian;
    _StopRadian = obj._StopRadian;

    return *this;
}

bool Arc::operator==(const Arc& obj) const
{
    return _Center == obj._Center && Math::FuzzyCompare(_Radius, obj._Radius) &&
           Math::FuzzyCompare(_StartRadian, obj._StartRadian) && Math::FuzzyCompare(_StopRadian, obj._StopRadian);
}

::ShapeViewer::Point& Arc::Center()
{
    return _Center;
}

const Point& Arc::Center() const
{
    return _Center;
}

::ShapeViewer::Point Arc::PointAtRadian(double radian) const
{
    return ::ShapeViewer::Point(std::cos(radian) * _Radius + _Center.X(), std::sin(radian) * _Radius + _Center.Y());
}

double Arc::Radius() const
{
    return _Radius;
}

void Arc::Radius(double value)
{
    _Radius = value;
}

double Arc::StartRadian() const
{
    return _StartRadian;
}

void Arc::StartRadian(double value)
{
    _StartRadian = value;
}

double Arc::StopRadian() const
{
    return _StopRadian;
}

void Arc::StopRadian(double value)
{
    _StopRadian = value;
}

double Arc::RestrictRadian(double Radian)
{
    return roundRadian(Radian);
}

double Arc::RestrictStartRadian() const
{
    return roundRadian(_StartRadian);
}

double Arc::RestrictStopRadian() const
{
    return roundRadian(_StopRadian);
}

bool Arc::Equal(const Geometry& obj) const
{
    return ShapeViewer::Equal(*this, obj);
}

bool Arc::Copy(const Geometry& obj)
{
    return ShapeViewer::Copy(*this, obj);
}

::ShapeViewer::Arc* Arc::Clone() const
{
    return ShapeViewer::Clone(*this);
}

::ShapeViewer::Point Arc::StartPoint() const
{
    return PointAtRadian(_StartRadian);
}

::ShapeViewer::Point Arc::StopPoint() const
{
    return PointAtRadian(_StopRadian);
}

double Arc::RotationRadian() const
{
    auto startRadian = roundRadian(_StartRadian);
    auto stopRadian = roundRadian(_StopRadian);

    if (stopRadian > startRadian)
    {
        return stopRadian - startRadian;
    }

    return pi2 - (startRadian - stopRadian);
}

bool Arc::LargerThanSemiCicle() const
{
    auto startRadian = roundRadian(_StartRadian);
    auto stopRadian = roundRadian(_StopRadian);

    if ((stopRadian - startRadian) > std::numbers::pi ||
        (stopRadian - startRadian) < 0 && (startRadian - stopRadian) < std::numbers::pi)
    {
        return true;
    }

    return false;
}
