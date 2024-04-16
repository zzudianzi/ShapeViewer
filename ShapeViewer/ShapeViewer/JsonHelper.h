#pragma once

#include "Polyline.h"

#include <string>

namespace ShapeViewer
{
class JsonHelper
{
  public:
    static Polyline ReadJson(const std::wstring& jsonString);
};
} // namespace ShapeViewer
