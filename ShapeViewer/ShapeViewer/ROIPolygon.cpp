#include "pch.h"
#include "ROIPolygon.h"

using namespace ShapeViewer;

ROIPolygon::ROIPolygon(const Polygon& polygon)
{
    _Polygon = polygon;
}

Point ROIPolygon::Center() const
{
    throw;
}

bool ROIPolygon::SamePos(const ROI& roi) const
{
    throw;
}

void ROIPolygon::MarkPositions(std::vector<Point>& markPositions) const
{
}

std::vector<Point> ROIPolygon::MarkPositions() const
{
    throw;
}

bool ROIPolygon::PtInShape(const Point& pt) const
{
    throw;
}

bool ROIPolygon::PtNearBoundary(const Point& pt, double maxDis) const
{
    throw;
}

bool ROIPolygon::DragMark(int selectedMark, const Point& oriPos, const Point& curPos, const ROI& oriROI)
{
    throw;
}
