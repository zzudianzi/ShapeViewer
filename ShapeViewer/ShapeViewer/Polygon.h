#pragma once

#include "Point.h"

namespace ShapeViewer
{
class Polygon : public Geometry
{
  public:
    using Vertices = std::vector<Point>;
    using Corner = Point;

    Polygon();
    explicit Polygon(const Vertices& vertices);
    explicit Polygon(const Vertices&& vertices);

    Polygon(const Polygon& obj);
    Polygon& operator=(const Polygon& polygon);
    bool operator==(const Polygon& obj) const;
    
    bool Equal(const Geometry& obj) const override;
    bool Copy(const Geometry& obj) override;
    Polygon* Clone() const override;

    Vertices& GetVertices();
    const Vertices& GetVertices() const;

    Point GetCenter() const;

    Corner GetCorner(int index) const;
    bool UpdateCorner(const Corner& corner, int index);

  private:
    Vertices _Vertices;
};

} // namespace ShapeViewer
