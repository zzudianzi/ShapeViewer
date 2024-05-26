#include "pch.h"

#include "Polyline.h"
#include "TemplateHelper.hpp"

using namespace ShapeViewer;

Polyline::Polyline()
{
}

Polyline::Polyline(const Vertices& vertices)
{
    _Vertices = vertices;
}

Polyline::Polyline(const Vertices&& vertices)
{
    _Vertices = std::move(vertices);
}

Polyline::Polyline(const Polyline& obj)
{
    *this = obj;
}

ShapeViewer::Polyline& Polyline::operator=(const Polyline& Polyline)
{
    _Vertices = Polyline._Vertices;

    return *this;
}

bool Polyline::operator==(const Polyline& obj) const
{
    return _Vertices == obj._Vertices;
}

bool Polyline::Equal(const Geometry& obj) const
{
    return ShapeViewer::Equal(*this, obj);
}

bool Polyline::Copy(const Geometry& obj)
{
    return ShapeViewer::Copy(*this, obj);
}

ShapeViewer::Polyline* Polyline::Clone() const
{
    return ShapeViewer::Clone(*this);
}

Polyline::Vertices& Polyline::GetVertices()
{
    return _Vertices;
}

const Polyline::Vertices& Polyline::GetVertices() const
{
    return _Vertices;
}

Point Polyline::GetCenter() const
{
    int count = (int)_Vertices.size();
    Point center;
    for (auto&& vertex : _Vertices)
    {
        center = center + vertex;
    }

    if (count > 0)
    {
        center.X(center.X() / (double)count);
        center.Y(center.Y() / (double)count);
    }

    return center;
}

Polyline::Corner Polyline::GetCorner(int index) const
{
    if (index < 0 || index >= _Vertices.size())
    {
        return {};
    }

    return _Vertices[index];
}

bool Polyline::UpdateCorner(const Corner& corner, int index)
{
    if (index < 0 || index >= _Vertices.size())
    {
        return false;
    }

    _Vertices[index] = corner;

    return true;
}
