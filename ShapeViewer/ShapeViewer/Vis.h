#pragma once

#include <d2d1_3.h>
#include <dwrite.h>
#include "Rect.h"

#include <optional>

namespace ShapeViewer
{
class Display;
class Vis
{
  public:
    Vis();
    explicit Vis(const Vis& vis);
    virtual ~Vis() = default;
    virtual void Draw() = 0;

    D2D1_COLOR_F Color() const;
    void Color(D2D1_COLOR_F value);

    bool Visible() const;
    void Visible(bool value);

    Display* GetDisplay();
    Display* GetDisplay() const;
    void SetDisplay(Display* display);

    virtual std::optional<::ShapeViewer::Rect> BoundingRect() const = 0;

    static constexpr double CrossHalfLength = 3.;

  protected:
    Display* _Display;
    bool _Visible;
    D2D1_COLOR_F _Color;
};
} // namespace ShapeViewer
