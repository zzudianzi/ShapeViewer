#include "pch.h"
#include "Circle.h"
#include "MathTool.h"
#include "TemplateHelper.hpp"

using namespace ShapeViewer;

Circle::Circle(const ShapeViewer::Point& center, double radius) : _Center(center), _Radius(radius)
{
}

Circle::Circle(const Circle& obj)
{
    _Center = obj._Center;
    _Radius = obj._Radius;
}

Circle::~Circle() = default;

Circle& Circle::operator=(const Circle& obj)
{
    _Center = obj._Center;
    _Radius = obj._Radius;

    return *this;
}

Point& Circle::Center()
{
    return _Center;
}

const Point& Circle::Center() const
{
    return _Center;
}

double Circle::Radius() const
{
    return _Radius;
}

void Circle::Radius(double value)
{
    _Radius = value;
}

bool Circle::PtInCircle(const ShapeViewer::Point& pt) const
{
    auto dis = Math::CalcDistanceFormPointToPoint(pt, _Center);

    return dis < _Radius;
}

bool Circle::operator==(const Circle& obj) const
{
    return _Center == obj._Center && _Radius == obj._Radius;
}

bool Circle::Equal(const Geometry& obj) const
{
    return ShapeViewer::Equal(*this, obj);
}

bool Circle::Copy(const Geometry& obj)
{
    return ShapeViewer::Copy(*this, obj);
}

Circle* Circle::Clone() const
{
    return ShapeViewer::Clone(*this);
}
