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
    virtual ~Overlay();

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

  private:
    Overlay(Display* display, bool scaling = true);
    std::vector<Vis*> _Items;
    Display* _Display;

    bool _Visible = true;
    bool _Scaling;

    int _ZOrder = 0;

    friend class Dispaly;
};
} // namespace ShapeViewer
