#pragma once

#include "Rect.h"

#include <vector>
#include <optional>

namespace ShapeViewer::Math
{
constexpr double Epsilon = 1e-8;
void CalcLineByPointAndAngle(double x, double y, double angle, double& a, double& b, double& c);
void CalcLineByPointAndAngle(const Point& pt, double angle, double& a, double& b, double& c);
bool CalcLineByTwoPoints(double x1, double y1, double x2, double y2, double& a, double& b, double& c);
bool CalcLineByTwoPoints(const Point& pt1, const Point& pt2, double& a, double& b, double& c);
bool CalcVerticalFootOfPointToline(double x, double y, double a, double b, double c, double& fX, double& fY);
bool CalcVerticalFootOfPointToline(const Point& point, double a, double b, double c, Point& foot);
bool CalcSymmetricPointOfLine(double x, double y, double a, double b, double c, double& mx, double& my);
bool CalcSymmetricPointOfLine(const Point& point, double a, double b, double c, Point& symPoint);
bool FuzzyCompare(double l, double r);
bool PtInPolygon(double x, double y, const std::vector<Point>& polygon);
bool PtInPolygon(const Point& pt, const std::vector<Point>& polygon);
void CalcVectorOfTwoPoints(double x1, double y1, double x2, double y2, double& vx, double& vy);
void CalcVectorOfTwoPoints(const Point& pt1, const Point& pt2, Point& vector);
Point CalcVectorOfTwoPoints(const Point& pt1, const Point& pt2);
double CalcAngleOfTwoVector(double vx1, double vy1, double vx2, double vy2);
double CalcAngleOfTwoVector(const Point& v1, const Point& v2);
bool UnitVector(double vx, double vy, double& ux, double& uy);
bool UnitVector(const Point& vector, Point& unitVector);
double CalcDistanceFormPointToLine(double x, double y, double a, double b, double c);
double CalcDistanceFormPointToLine(const Point& pt, double a, double b, double c);
double CalcDistanceFormPointToPoint(const Point& pt1, const Point& pt2);
bool PointNearLineSegment(const Point& point, const Point& pt1, const Point& pt2, double disMax);
bool PointNearBoundary(const Point& point, const std::vector<Point>& vertices, double maxDis);
std::optional<::ShapeViewer::Rect> CalcBoundingRect(const std::vector<::ShapeViewer::Point>& points);
} // namespace ShapeViewer::Math
