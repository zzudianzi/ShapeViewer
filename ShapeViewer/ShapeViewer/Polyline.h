#pragma once

#include "CommonStructure.h"
#include "Geometry.h"

#include <vector>

namespace ShapeViewer
{
class Polyline
{
  public:
    using Vertices = std::vector<Vertex>;

    Vertices& Points();
    const Vertices& Points() const;

  private:
    Vertices _Points;
};
} // namespace ShapeViewer
