#pragma once

#include "Vis.h"

#include <string>

namespace ShapeViewer
{
class VisText : public ::ShapeViewer::Vis
{
  public:
    enum class Position
    {
        TopLeft,
        TopRight,
        BottomRight,
        BottomLeft,
        Custom
    };

    enum class CoordinateSystem
    {
        Window,
        Scene
    };

    explicit VisText(const std::wstring& text);

    const std::wstring& Text() const;
    std::wstring& Text();

    Position GetPosition() const;
    void SetPosition(Position value);

    CoordinateSystem GetCoordinateSystem() const;
    void SetCoordinateSystem(CoordinateSystem value);

    void Draw() override;

    std::optional<::ShapeViewer::Rect> BoundingRect() const;

  private:
    std::wstring _Text;
    Position _Position;
    CoordinateSystem _CS;
    winrt::com_ptr<IDWriteTextLayout> _TextLayout;
};
} // namespace ShapeViewer
