#pragma once

#include "Point.h"

#include <vector>

namespace ShapeViewer
{
class Polyline : public Geometry
{
  public:
    using Vertices = std::vector<Point>;
    using Corner = Point;

    Polyline();
    explicit Polyline(const Vertices& vertices);
    explicit Polyline(const Vertices&& vertices);

    Polyline(const Polyline& obj);
    Polyline& operator=(const Polyline& Polyline);
    bool operator==(const Polyline& obj) const;

    bool Equal(const Geometry& obj) const override;
    bool Copy(const Geometry& obj) override;
    Polyline* Clone() const override;

    Vertices& GetVertices();
    const Vertices& GetVertices() const;

    Point GetCenter() const;

    Corner GetCorner(int index) const;
    bool UpdateCorner(const Corner& corner, int index);

  private:
    Vertices _Vertices;
};
} // namespace ShapeViewer
