#include "pch.h"
#include "Rect.h"

#include "MathTool.h"

using namespace ShapeViewer;

Rect::Rect() : _Angle(0.)
{
}

Rect::Rect(double xs, double ys, double xe, double ye, double angle) : _St(xs, ys), _Ed(xe, ye), _Angle(angle)
{
}

Rect::Rect(Point center, double width, double height, double angle)
{
    auto halfW = width * 0.5;
    auto halfH = height * 0.5;

    _St = Point(
        center.X() - halfW * std::cos(angle) + halfH * std::sin(angle),
        center.Y() - halfW * std::sin(angle) - halfH * std::cos(angle));
    _Ed = Point(center.X() * 2 - _St.X(), center.Y() * 2 - _St.Y());
    _Angle = angle;
}

Rect::Rect(Point st, Point ed, double angle)
{
    _St = st;
    _Ed = ed;
    _Angle = angle;
}

Rect::Rect(const Rect& obj)
{
    _St = obj._St;
    _Ed = obj._Ed;
    _Angle = obj._Angle;
}

Point& Rect::St()
{
    return _St;
}

const Point& Rect::St() const
{
    return _St;
}

Point& Rect::Ed()
{
    return _Ed;
}

const Point& Rect::Ed() const
{
    return _Ed;
}

Point Rect::Center() const
{
    return Point((_St.X() + _Ed.X()) * 0.5, (_St.Y() + _Ed.Y()) * 0.5);
}

double Rect::Angle() const
{
    return _Angle;
}

void Rect::Angle(double value)
{
    _Angle = value;
}

std::vector<Point> Rect::Corners() const
{
    std::vector<Point> corners;
    Corners(corners);
    return corners;
}

void Rect::Corners(std::vector<Point>& corners) const
{
    corners.clear();
    corners.resize(4);

    double a, b, c;
    Math::CalcLineByPointAndAngle(Center(), _Angle, a, b, c);

    Point topRight, bottomLeft;
    Math::CalcSymmetricPointOfLine(_St, a, b, c, bottomLeft);
    Math::CalcSymmetricPointOfLine(_Ed, a, b, c, topRight);

    corners[(int)Corner::TopLeft] = _St;
    corners[(int)Corner::TopRight] = topRight;
    corners[(int)Corner::BottomRight] = _Ed;
    corners[(int)Corner::BottomLeft] = bottomLeft;
}

double Rect::Width() const
{
    double a, b, c;
    Math::CalcLineByPointAndAngle(Center(), _Angle, a, b, c);

    Point topRight;
    Math::CalcSymmetricPointOfLine(_Ed, a, b, c, topRight);

    return Math::CalcDistanceFormPointToPoint(_St, topRight);
}

double Rect::Height() const
{
    double a, b, c;
    Math::CalcLineByPointAndAngle(Center(), _Angle, a, b, c);

    Point bottomLeft;
    Math::CalcSymmetricPointOfLine(_St, a, b, c, bottomLeft);

    return Math::CalcDistanceFormPointToPoint(_St, bottomLeft);
}

bool Rect::PtInRect(const Point& pt) const
{
    throw;
}

void Rect::SetRectByCorners(
    const Point& topLeft, const Point& topRight, const Point& bottomRight, const Point& bottomLeft)
{
    _St = topLeft;
    _Ed = bottomRight;

    auto leftCenter = (topLeft + bottomLeft) * 0.5;
    auto rightCenter = (topRight + bottomRight) * 0.5;

    _Angle = std::atan2(rightCenter.Y() - leftCenter.Y(), rightCenter.X() - leftCenter.X());
}

bool Rect::operator==(const Geometry& obj) const
{
    auto object = dynamic_cast<const Rect*>(&obj);
    if (!object)
    {
        return false;
    }

    if (this == object)
    {
        return true;
    }

    return _St == object->_St && _Ed == object->_Ed && Math::FuzzyCompare(_Angle, object->_Angle);
}

bool Rect::Copy(const Geometry& obj)
{
    auto object = dynamic_cast<const Rect*>(&obj);
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

Rect* Rect::Clone() const
{
    return new Rect(*this);
}

bool ShapeViewer::operator == (const Rect& l, const Rect& r)
{
    return l.operator==(r);
}