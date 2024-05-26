#include "pch.h"
#include "Point.h"
#include "MathTool.h"
#include "TemplateHelper.hpp"

using namespace ShapeViewer;

Point::Point() : _X(0.), _Y(0.)
{
}

Point::Point(double x, double y)
{
    _X = x;
    _Y = y;
}

Point::Point(const Point& obj)
{
    *this = obj;
}

Point& Point::operator=(const Point& obj)
{
    _X = obj._X;
    _Y = obj._Y;

    return *this;
}

bool Point::operator==(const Point& obj) const
{
    return std::abs(_X - obj._X) < Math::Epsilon && std::abs(_Y - obj._Y) < Math::Epsilon;
}

bool Point::Equal(const Geometry& obj) const
{
    return ShapeViewer::Equal(*this, obj);
}

bool Point::Copy(const Geometry& obj)
{
    auto object = dynamic_cast<const Point*>(&obj);
    if (!object)
    {
        return false;
    }

    if (this == object)
    {
        return true;
    }

    *this = *object;

    return true;
}

Point* Point::Clone() const
{
    return new Point(*this);
}

double Point::X() const
{
    return _X;
}

void Point::X(double value)
{
    _X = value;
}

double Point::Y() const
{
    return _Y;
}

void Point::Y(double value)
{
    _Y = value;
}

Point Point::operator*(double value) const
{
    return {_X * value, _Y * value};
}

Point Point::operator+(const Point& obj) const
{
    return {_X + obj._X, _Y + obj._Y};
}

Point Point::operator-(const Point& obj) const
{
    return {_X - obj._X, _Y - obj._Y};
}