#pragma once

#include <vector>
#include <Vis.h>

namespace ShapeViewer
{
class Display;
class Geometry;
class Overlay
{
  public:
    Overlay(Display* display, bool scaling = true);
    virtual ~Overlay();

    Display* GetDisplay();
    const Display* GetDisplay() const;
    void SetDisplay(Display* display);

    bool Visible() const;
    void Visible(bool value);

    bool Scaling() const;
    void Scaling(bool value);

    void AddItem(Geometry* geometry);
    void AddItem(Vis* vis);

    Vis* GetItem(int index);

    bool DeleteItem(Vis* vis);
    bool DeleteItem(int index);

    void DeleteAllItems();

    int GetItemCount();

    void Draw();

  private:
    std::vector<Vis*> _Items;
    Display* _Display;

    bool _Visible = true;
    bool _Scaling;

    int _ZOrder = 0;
};
} // namespace ShapeViewer
