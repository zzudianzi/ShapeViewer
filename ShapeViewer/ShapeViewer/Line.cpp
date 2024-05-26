#include "pch.h"
#include "Line.h"
#include "MathHelper.h"
#include "TemplateHelper.hpp"
#include "MathTool.h"

using namespace ShapeViewer;

Line::Line(const ::ShapeViewer::Point& st, const ::ShapeViewer::Point& ed)
{
    Math::CalcLineByTwoPoints(st, ed, _A, _B, _C);
}

Line::Line(double a, double b, double c) : _A(a), _B(b), _C(c)
{

}

Line::Line(const Line& obj) : _A(obj._A), _B(obj._B), _C(obj._C)
{

}

Line& Line::operator=(const Line& obj)
{
    _A = obj._A;
    _B = obj._B;
    _C = obj._C;

    return *this;
}

bool Line::operator==(const Line& obj) const
{
    return Math::FuzzyCompare(_A, obj._A) && Math::FuzzyCompare(_B, obj._B) && Math::FuzzyCompare(_C, obj._C);
}

double Line::A() const
{
    return _A;
}

void Line::A(double value)
{
    _A = value;
}

double Line::B() const
{
    return _B;
}

void Line::B(double value)
{
    _B = value;
}

double Line::C() const
{
    return _C;
}

void Line::C(double value)
{
    _C = value;
}

bool Line::Equal(const Geometry& obj) const
{
    return ShapeViewer::Equal(*this, obj);
}

bool Line::Copy(const Geometry& obj)
{
    return ShapeViewer::Copy(*this, obj);
}

Line* Line::Clone() const
{
    return ShapeViewer::Clone(*this);
}
