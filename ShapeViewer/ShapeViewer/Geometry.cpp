#include "pch.h"
#include "Geometry.h"

using namespace ShapeViewer;

Geometry::~Geometry() = default;

bool Geometry::operator != (const Geometry& obj) const
{
    return !Equal(obj);
}