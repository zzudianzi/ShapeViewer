#pragma once

namespace ShapeViewer
{

class Geometry
{
    virtual bool operator==(const Geometry& obj) const = 0;
    bool operator!=(const Geometry& obj) const;
    virtual bool Copy(const Geometry& obj) = 0;
    virtual Geometry* Clone() const = 0;
};

} // namespace ShapeViewer
