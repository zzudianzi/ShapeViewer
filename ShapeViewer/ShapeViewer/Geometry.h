#pragma once

namespace ShapeViewer
{

class Geometry
{
  public:
    virtual ~Geometry();
    bool operator!=(const Geometry& obj) const;
    virtual bool Equal(const Geometry& obj) const = 0;
    virtual bool Copy(const Geometry& obj) = 0;
    virtual Geometry* Clone() const = 0;
};

} // namespace ShapeViewer
