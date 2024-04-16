#include "pch.h"

#include "Polyline.h"

using namespace ShapeViewer;

Polyline::Vertices& Polyline::Points()
{
    return _Points;
}

const Polyline::Vertices& Polyline::Points() const
{
    return _Points;
}