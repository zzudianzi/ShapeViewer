#pragma once

#include <d2d1_3.h>
#include "Rect.h"

#include <optional>

namespace ShapeViewer
{
class Display;
class Vis
{
  public:
    Vis();
    virtual ~Vis() = default;
    virtual void Draw(ID2D1RenderTarget* rt, ID2D1SolidColorBrush* brush) = 0;

    D2D1_COLOR_F Color() const;
    void Color(D2D1_COLOR_F value);

    bool Visible() const
    {
        return _Visible;
    }

    void Visible(bool value)
    {
        _Visible = value;
    }

    Display* GetDisplay();
    const Display* GetDisplay() const;
    void SetDisplay(Display* display);

    virtual std::optional<::ShapeViewer::Rect> BoundingRect() const = 0;

  protected:
    Display* _Display;
    bool _Visible;
    D2D1_COLOR_F _Color;
};
} // namespace ShapeViewer
