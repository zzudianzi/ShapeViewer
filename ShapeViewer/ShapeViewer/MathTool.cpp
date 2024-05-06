#include "pch.h"
#include "MathTool.h"

#include <algorithm>

namespace ShapeViewer::Math
{
bool FuzzyCompare(double l, double r)
{
    return std::abs(l - r) < Epsilon;
}

void CalcLineByPointAndAngle(double x, double y, double angle, double& a, double& b, double& c)
{
    auto x1 = x + std::cos(angle) * 100.;
    auto y1 = y + std::sin(angle) * 100.;
    if (std::abs(x1 - x) < std::abs(y1 - y))
    {
        a = 1.;
        b = (x1 - x) / (y - y1);
        c = -x - b * y;
    }
    else
    {
        a = (y1 - y) / (x - x1);
        b = 1.;
        c = -y - a * x;
    }
}

void CalcLineByPointAndAngle(const Point& pt, double angle, double& a, double& b, double& c)
{
    CalcLineByPointAndAngle(pt.X(), pt.Y(), angle, a, b, c);
}

bool CalcLineByTwoPoints(double x1, double y1, double x2, double y2, double& a, double& b, double& c)
{
    auto vx = x2 - x1;
    auto vy = y2 - y1;
    if (std::abs(vx) < Math::Epsilon && std::abs(vy) < Math::Epsilon)
    {
        a = b = c = 0.;
        return false;
    }

    if (std::abs(vx) > std::abs(vy))
    {
        a = -vy / vx;
        b = 1.;
        c = -a * x1 - y1;
    }
    else
    {
        a = 1.;
        b = -vx / vy;
        c = -b * y1 - x1;
    }

    return true;
}

bool CalcLineByTwoPoints(const Point& pt1, const Point& pt2, double& a, double& b, double& c)
{
    return CalcLineByTwoPoints(pt1.X(), pt1.Y(), pt2.X(), pt2.Y(), a, b, c);
}

bool CalcVerticalFootOfPointToline(double x, double y, double a, double b, double c, double& fX, double& fY)
{
    if (std::abs(a) < Math::Epsilon && std::abs(b) < Math::Epsilon)
        return false;

    fX = (b * b * x - a * b * y - a * c) / (a * a + b * b);
    fY = (a * a * y - b * c - a * b * x) / (b * b + a * a);

    return true;
}

bool CalcVerticalFootOfPointToline(const Point& point, double a, double b, double c, Point& foot)
{
    double fx, fy;
    if (!CalcVerticalFootOfPointToline(point.X(), point.Y(), a, b, c, fx, fy))
        return false;
    foot.X(fx);
    foot.Y(fy);
    return true;
}

bool CalcSymmetricPointOfLine(double x, double y, double a, double b, double c, double& mx, double& my)
{
    double fX, fY;
    if (!CalcVerticalFootOfPointToline(x, y, a, b, c, fX, fY))
        return false;

    mx = fX * 2. - x;
    my = fY * 2. - y;

    return true;
}

bool CalcSymmetricPointOfLine(const Point& point, double a, double b, double c, Point& symPoint)
{
    double symX, symY;
    if (!CalcSymmetricPointOfLine(point.X(), point.Y(), a, b, c, symX, symY))
        return false;

    symPoint.X(symX);
    symPoint.Y(symY);
    return true;
}

bool PtInPolygon(double x, double y, const std::vector<Point>& polygon)
{
    Point q[4];
    for (int i = 0; i < (std::min)(4, (int)polygon.size()); i++)
    {
        q[i] = polygon[i];
    }

    auto count = static_cast<int>(polygon.size());
    if (count < 3)
        return false;

    auto ret = false;
    for (int i = 0, j = count - 1; i < count; j = i++)
    {
        if (((polygon[i].Y() > y) != (polygon[j].Y() > y)) &&
            (x < (polygon[j].X() - polygon[i].X()) * (y - polygon[i].Y()) / (polygon[j].Y() - polygon[i].Y()) +
                     polygon[i].X()))
        {
            ret = !ret;
        }
    }
    return ret;
}

bool PtInPolygon(const Point& pt, const std::vector<Point>& polygon)
{
    return PtInPolygon(pt.X(), pt.Y(), polygon);
}

void CalcVectorOfTwoPoints(double x1, double y1, double x2, double y2, double& vx, double& vy)
{
    vx = x2 - x1;
    vy = y2 - y1;
}

void CalcVectorOfTwoPoints(const Point& pt1, const Point& pt2, Point& vector)
{
    vector = pt2 - pt1;
}

Point CalcVectorOfTwoPoints(const Point& pt1, const Point& pt2)
{
    Point vector;
    CalcVectorOfTwoPoints(pt1, pt2, vector);
    return vector;
}

double CalcAngleOfTwoVector(double vx1, double vy1, double vx2, double vy2)
{
    // consider the cross product: sin = a x b / |a| * |b|, and dot product std::cosx = a ? b / |a| * |b|
    // x = atan2(sin x, std::cos x)

    return std::atan2(vx1 * vy2 - vy1 * vx2, vx1 * vy1 + vx2 * vy2);
}

double CalcAngleOfTwoVector(const Point& v1, const Point& v2)
{
    return CalcAngleOfTwoVector(v1.X(), v1.Y(), v2.X(), v2.Y());
}

bool UnitVector(double vx, double vy, double& ux, double& uy)
{
    auto d = std::sqrt(ux * ux + uy * uy);
    if (d < Math::Epsilon)
    {
        ux = uy = 0.;
        return false;
    }

    ux = vx / d;
    uy = vy / d;

    return true;
}

bool UnitVector(const Point& vector, Point& unitVector)
{
    double ux, uy;
    if (!UnitVector(vector.X(), vector.Y(), ux, uy))
        return false;
    unitVector.X(ux);
    unitVector.Y(uy);
    return true;
}

double CalcDistanceFormPointToLine(double x, double y, double a, double b, double c)
{
    auto fX = (b * b * x - a * b * y - a * c) / (a * a + b * b);
    auto fY = (a * a * y - b * c - a * b * x) / (b * b + a * a);

    return std::sqrt(pow(x - fX, 2) + pow(y - fY, 2));
}

double CalcDistanceFormPointToLine(const Point& pt, double a, double b, double c)
{
    return CalcDistanceFormPointToLine(pt.X(), pt.Y(), a, b, c);
}

double CalcDistanceFormPointToPoint(const Point& pt1, const Point& pt2)
{
    auto vector = pt2 - pt1;
    return std::sqrt(vector.X() * vector.X() + vector.Y() * vector.Y());
}

bool PointNearLineSegment(const Point& point, const Point& pt1, const Point& pt2, double disMax)
{
    double a, b, c;
    CalcLineByTwoPoints(pt1, pt2, a, b, c);
    auto dis = CalcDistanceFormPointToLine(point, a, b, c);
    if (dis > disMax)
        return false;
    Point vector;
    CalcVectorOfTwoPoints(pt1, pt2, vector);
    Point unitVector;
    UnitVector(vector, unitVector);
    Point perpendicular(vector.Y(), -vector.X());
    auto angle = std::atan2(perpendicular.Y(), perpendicular.X());
    auto offset = unitVector * disMax;
    double a1, b1, c1, a2, b2, c2;
    CalcLineByPointAndAngle(pt1 - offset, angle, a1, b1, c1);
    CalcLineByPointAndAngle(pt2 + offset, angle, a2, b2, c2);

    auto d1 = a1 * point.X() + b1 * point.Y() + c1;
    auto d2 = a2 * point.X() + b2 * point.Y() + c2;

    return d1 * d2 < 0;
}

bool PointNearBoundary(const Point& point, const std::vector<Point>& vertices, double maxDis)
{
    int size = (int)vertices.size();
    for (int i = 0; i < size; i++)
    {
        const auto& pt1 = vertices[(i + size - 1) % size];
        const auto& pt2 = vertices[i];
        if (Math::PointNearLineSegment(point, pt1, pt2, maxDis))
        {
            return true;
        }
    }

    return false;
}

std::optional<::ShapeViewer::Rect> CalcBoundingRect(const std::vector<::ShapeViewer::Point>& points)
{
    if (points.empty())
    {
        return {};
    }
    auto rc = std::make_optional<::ShapeViewer::Rect>(points[0].X(), points[0].Y(), points[0].X(), points[0].Y());

    for (int i = 1; i < (int)points.size(); i++)
    {
        if (points[i].X() < rc->St().X())
        {
            rc->St().X(points[i].X());
        }
        if (points[i].Y() < rc->St().Y())
        {
            rc->St().Y(points[i].Y());
        }
        if (points[i].X() > rc->Ed().X())
        {
            rc->Ed().X(points[i].X());
        }
        if (points[i].Y() > rc->Ed().Y())
        {
            rc->Ed().Y(points[i].Y());
        }
    }

    return rc;
}
} // namespace ShapeViewer::Math
