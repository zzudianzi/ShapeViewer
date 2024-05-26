#include "pch.h"
#include "Ellipse.h"
#include "MathTool.h"

#include "TemplateHelper.hpp"

using namespace ShapeViewer;

Ellipse::Ellipse(const ShapeViewer::Point& center, double a, double b, double angle)
    : _Center(center), _A(a), _B(b), _Angle(angle)
{
}

Ellipse::Ellipse(const Ellipse& obj)
{
    _Center = obj._Center;
    _A = obj._A;
    _B = obj._B;
    _Angle = obj._Angle;
}

Ellipse::~Ellipse() = default;

::ShapeViewer::Ellipse& Ellipse::operator=(const Ellipse& obj)
{
    _Center = obj._Center;
    _A = obj._A;
    _B = obj._B;
    _Angle = obj._Angle;

    return *this;
}

ShapeViewer::Point& Ellipse::Center()
{
    return _Center;
}

const ShapeViewer::Point& Ellipse::Center() const
{
    return _Center;
}

double Ellipse::A() const
{
    return _A;
}

void Ellipse::A(double value)
{
    _A = value;
}

double Ellipse::B() const
{
    return _B;
}

void Ellipse::B(double value)
{
    _B = value;
}

bool Ellipse::PtInEllipse(const ShapeViewer::Point& pt)
{
    auto c = std::sqrt(std::abs(_A * _A - _B * _B));
    auto vector = Point(std::cos(_Angle), sin(_Angle)) * c;
    auto focus1 = _Center - vector, focus2 = _Center + vector;

    auto dis1 = Math::CalcDistanceFormPointToPoint(pt, focus1);
    auto dis2 = Math::CalcDistanceFormPointToPoint(pt, focus2);

    return (dis1 + dis2) < 2 * _A;
}

bool Ellipse::operator==(const Ellipse& obj) const
{
    return _Center == obj._Center && Math::FuzzyCompare(_A, obj._A) && Math::FuzzyCompare(_B, obj._B) &&
           Math::FuzzyCompare(_Angle, obj._Angle);
}

bool Ellipse::Equal(const Geometry& obj) const
{
    return ShapeViewer::Equal(*this, obj);
}

bool Ellipse::Copy(const Geometry& obj)
{
    return ShapeViewer::Copy(*this, obj);
}

Geometry* Ellipse::Clone() const
{
    return ShapeViewer::Clone(*this);
}
