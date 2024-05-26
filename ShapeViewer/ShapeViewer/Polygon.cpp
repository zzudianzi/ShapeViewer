#include "pch.h"
#include "Polygon.h"
#include "TemplateHelper.hpp"

using namespace ShapeViewer;

Polygon::Polygon()
{
}

Polygon::Polygon(const Vertices& vertices)
{
    _Vertices = vertices;
}

Polygon::Polygon(const Vertices&& vertices)
{
    _Vertices = std::move(vertices);
}

Polygon::Polygon(const Polygon& obj)
{
    *this = obj;
}

ShapeViewer::Polygon& Polygon::operator=(const Polygon& polygon)
{
    _Vertices = polygon._Vertices;

    return *this;
}

bool Polygon::operator==(const Polygon& obj) const
{
    return _Vertices == obj._Vertices;
}

bool Polygon::Equal(const Geometry& obj) const
{
    return ShapeViewer::Equal(*this, obj);
}

bool Polygon::Copy(const Geometry& obj)
{
    return ShapeViewer::Copy(*this, obj);
}

ShapeViewer::Polygon* Polygon::Clone() const
{
    return ShapeViewer::Clone(*this);
}

Polygon::Vertices& Polygon::GetVertices()
{
    return _Vertices;
}

const Polygon::Vertices& Polygon::GetVertices() const
{
    return _Vertices;
}

Point Polygon::GetCenter() const
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

Polygon::Corner Polygon::GetCorner(int index) const
{
    if (index < 0 || index >= _Vertices.size())
    {
        return {};
    }

    return _Vertices[index];
}

bool Polygon::UpdateCorner(const Corner& corner, int index)
{
    if (index < 0 || index >= _Vertices.size())
    {
        return false;
    }

    _Vertices[index] = corner;

    return true;
}
