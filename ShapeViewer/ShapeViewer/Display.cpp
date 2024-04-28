#include "pch.h"
#include "Display.h"
#include "Overlay.h"
#include "MathTool.h"

using namespace ShapeViewer;

Display::Display()
{
    _TransformToScene = D2D1::Matrix3x2F::Identity();
    _TransformToWindow = D2D1::Matrix3x2F::Identity();
}

Display::~Display()
{
    DeleteAllOverlays();
    DeleteAllROIs();
}

void Display::SetSceneRect(const Rect& rc)
{
    throw;
}

bool Display::FitSize()
{
    // Calculate the bounding box of all the visible items
    auto rect = [this]() -> std::optional<::ShapeViewer::Rect> {
        std::optional<::ShapeViewer::Rect> rc;
        for (auto&& roi : _ROIs)
        {
            if (std::optional<::ShapeViewer::Rect> r = roi->GetVis().BoundingRect())
            {
                if (!r)
                {
                    continue;
                }
                if (!rc)
                {
                    rc = r;
                    continue;
                }

                *rc |= *r;
            }
        }
        return rc;
    }();

    if (!rect)
    {
        return false;
    }

    D2D1_SIZE_F size = _D2D1RenderTarget->GetSize();
    double sceneWidth = rect->Ed().X() - rect->St().X();
    double sceneHeight = rect->Ed().Y() - rect->St().Y();

    if (sceneHeight < Math::Epsilon || sceneWidth < Math::Epsilon || size.width < 10. || size.height < 10.)
    {
        return false;
    }

    double scale = 1.;
    if (sceneWidth / sceneHeight > size.width / size.height)
    {
        scale = sceneWidth / size.width;
    }
    else
    {
        scale = sceneHeight / size.height;
    }

    auto scaleMatrix = D2D1::Matrix3x2F::Scale(scale, scale);
    D2D1_POINT_2F windowCenter = D2D1::Point2F(size.width * 0.5 * scale, size.height * 0.5 * scale);
    D2D1_POINT_2F sceneCenter =
        D2D1::Point2F((rect->St().X() + rect->Ed().X()) * 0.5, (rect->St().Y() + rect->Ed().Y()) * 0.5);
    auto translateMatrix = D2D1::Matrix3x2F::Translation(sceneCenter.x - windowCenter.x, sceneCenter.y - windowCenter.y);

    _TransformToScene = scaleMatrix * translateMatrix;
    _TransformToWindow = _TransformToScene;
    _TransformToWindow.Invert();

    for (auto&& roi : _ROIs)
    {
        CreateD2DFigure(roi->GetVis());
    }

    return true;
}

Overlay* Display::CreateOverlay(bool scaling)
{
    throw;
}

bool Display::DeleteOverlay(Overlay* overlay)
{
    if (!overlay)
    {
        return false;
    }

    auto iter = std::find(_Overlays.begin(), _Overlays.end(), overlay);
    if (iter == _Overlays.end())
    {
        return false;
    }

    delete overlay;
    _Overlays.erase(iter);
    return true;
}

bool Display::DeleteOverlay(int index)
{
    return DeleteOverlay(GetOverlay(index));
}

void Display::DeleteAllOverlays()
{
    for (auto overlay : _Overlays)
    {
        delete overlay;
    }

    _Overlays.clear();
}

int Display::GetOverlayCount() const
{
    return (int)_Overlays.size();
}

Overlay* Display::GetOverlay(int index)
{
    if (index < 0 || index >= _Overlays.size())
    {
        return nullptr;
    }

    return _Overlays[index];
}

int Display::AddROI(ROI* roi)
{
    _ROIs.push_back(roi);
    roi->SetDisplay(this);
    CreateD2DFigure(roi->GetVis());

    return (int)_ROIs.size() - 1;
}

bool Display::DeleteROI(ROI* roi)
{
    auto pos = std::find(_ROIs.begin(), _ROIs.end(), roi);
    if (pos == _ROIs.end())
    {
        return false;
    }

    _ROIs.erase(pos);
    return true;
}

bool Display::DeleteROI(int index)
{
    if (index < 0 || index >= (int)_ROIs.size())
    {
        return false;
    }

    _ROIs.erase(_ROIs.begin() + index);
    return true;
}

bool Display::DeleteAllROIs()
{
    for (auto roi : _ROIs)
    {
        delete roi;
    }

    _ROIs.clear();

    return true;
}

int Display::GetROICount() const
{
    return (int)_ROIs.size();
}

const D2D1::Matrix3x2F& Display::TransformToScene() const
{
    return _TransformToScene;
}

D2D1::Matrix3x2F& Display::TransformToScene()
{
    return _TransformToScene;
}

void Display::TransformToScene(const D2D1::Matrix3x2F& value)
{
    _TransformToScene = value;
}

const D2D1::Matrix3x2F& Display::TransformToWindow() const
{
    return _TransformToWindow;
}

D2D1::Matrix3x2F& Display::TransformToWindow()
{
    return _TransformToWindow;
}

void Display::TransformToWindow(const D2D1::Matrix3x2F& value)
{
    _TransformToWindow = value;
}

void Display::Draw()
{
    for (auto&& roi : _ROIs)
    {
        roi->Draw(_D2D1RenderTarget.get(), _Brush.get(), _TransformToWindow);
    }
}

void Display::OnMouseDown(WPARAM btnState, int x, int y)
{
    _ROIDrag = nullptr;
    _DragMark = 0;
    _OriDragROI.reset(nullptr);
    _DragMode = DragMode::None;

    double scale = _TransformToScene.m11;
    if (scale < Math::Epsilon)
    {
        return;
    }

    auto pt = ToScenePoint(x, y);
    double minDis = NearRadius / scale;
    double minDis2 = minDis * minDis;

    for (auto&& roi : _ROIs)
    {
        auto markPositions = roi->MarkPositions();
        int count = (int)markPositions.size();
        
        for (int i = 0; i < count; i++)
        {
            auto v = pt - markPositions[i];
            auto dis = v.X() * v.X() + v.Y() * v.Y();
            if (dis < minDis2)
            {
                minDis2 = dis;
                _DragMark = i;
                _ROIDrag = roi;
                _OriDragROI.reset(roi->Clone());
            }
        }
    }

    if (_ROIDrag)
    {
        _ROIDrag->DrawMark(true);
        _DragMode = DragMode::ROI;
        _Anchor = pt;
    }
}

void Display::OnMouseUp(WPARAM btnState, int x, int y)
{
    if (_DragMode == DragMode::ROI)
    {
        _DragMode = DragMode::None;
        _DragMark = 0;
        _ROIDrag->DrawMark(false);
        _ROIDrag = nullptr;
        _OriDragROI.reset(nullptr);
    }
}

void Display::OnMouseMove(WPARAM btnState, int x, int y)
{
    auto pt = ToScenePoint(x, y);
    if (_DragMode == DragMode::ROI && _ROIDrag)
    {
        _ROIDrag->DragMark(_DragMark, _Anchor, pt, *_OriDragROI);
    }
    else
    {
        double scale = _TransformToScene.m11;
        if (scale < Math::Epsilon)
        {
            return;
        }

        auto pt = ToScenePoint(x, y);
        double masDis = NearRadius / scale;
        bool find = false;
        for (auto&& roi : _ROIs)
        {
            if (find)
            {
                roi->DrawMark(false);
                continue;
            }

            find = roi->PtNearBoundary(pt, masDis);
            if (find)
            {
                roi->DrawMark(true);
            }
        }
    }
}

void Display::OnMouseWheel(WPARAM btnState, int x, int y, int delta)
{
    if (std::abs((int)(_Anchor.X() + 0.5) - x) > 5. || std::abs((int)(_Anchor.Y() + 0.5) - y) > 5.)
    {
        _Anchor.X(x);
        _Anchor.Y(y);
    }

    auto pt = _TransformToScene.TransformPoint(D2D1::Point2F(_Anchor.X(), _Anchor.Y()));
    auto scale = delta > 0 ? 0.9 : 1.1;
    auto scaleMatrix = D2D1::Matrix3x2F::Scale(scale, scale, pt);

    _TransformToScene = _TransformToScene * scaleMatrix;
    _TransformToWindow = _TransformToScene;
    _TransformToWindow.Invert();

    for (auto&& roi : _ROIs)
    {
        CreateD2DFigure(roi->GetVis());
    }
}

bool Display::CreateD2DFigure(Vis& vis)
{
    return vis.CreateD2DFigure(_D2D1Factory.get(), _TransformToWindow);
}

::ShapeViewer::Point Display::ToScenePoint(int x, int y)
{
    auto pointer = D2D1::Point2F((float)x, (float)y);
    auto pointerInScene = _TransformToScene.TransformPoint(pointer);
    return {pointerInScene.x, pointerInScene.y};
}