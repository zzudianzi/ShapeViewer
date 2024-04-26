#pragma once

#include <d2d1.h>

namespace ShapeViewer
{
	class Vis
	{
      public:
        virtual ~Vis() = default;
        virtual void Draw(ID2D1RenderTarget* rt, ID2D1Brush* brush) = 0;
	};
}