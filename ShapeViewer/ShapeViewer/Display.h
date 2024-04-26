#pragma once

#include "Rect.h"

namespace ShapeViewer
{
class Overlay;
class Display
{
  public:
    Display();
    ~Display();

    void SetSceneRect(const Rect& rc);

    bool FitSize(bool width = true, bool height = true);

    Overlay* CreateOverlay(bool scaling = true);
    bool DeleteOverlay(Overlay* overlay);
    bool DeleteOverlay(int index);
    void DeleteAllOverlays();
    int GetOverlayCount() const;
    Overlay* GetOverlay(int index);

  private:
    std::vector<Overlay*> _Overlays;
    double _Scale;
    Point _Translation;
};
} // namespace ShapeViewer
