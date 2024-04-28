#include "pch.h"
#include "Geometry.h"

using namespace ShapeViewer;

bool Geometry::operator != (const Geometry& obj) const
{
    return !operator==(obj);
}