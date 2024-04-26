#include "pch.h"
#include "Polygon.h"

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

bool Polygon::operator==(const Geometry& obj) const
{
    auto object = dynamic_cast<const Polygon*>(&obj);
    if (!object)
    {
        return false;
    }

    if (this == object)
    {
        return true;
    }

    return _Vertices == object->_Vertices;
}

bool Polygon::Copy(const Geometry& obj)
{
    auto object = dynamic_cast<const Polygon*>(&obj);
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

ShapeViewer::Polygon* Polygon::Clone() const
{
    return new Polygon(*this);
}

Polygon::Vertices& Polygon::GetVertices()
{
    return _Vertices;
}

const Polygon::Vertices& Polygon::GetVertices() const
{
    return _Vertices;
}

Polygon::Corner Polygon::GetCorner(int index)
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
