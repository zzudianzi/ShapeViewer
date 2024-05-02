#include "pch.h"
#include "Display.h"
#include "Overlay.h"
#include "MathTool.h"

using namespace ShapeViewer;

Display::Display() : _DragMark(0)
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

    auto scaleMatrix = D2D1::Matrix3x2F::Scale((float)scale, (float)scale);
    D2D1_POINT_2F windowCenter = D2D1::Point2F((float)(size.width * 0.5 * scale), (float)(size.height * 0.5 * scale));
    D2D1_POINT_2F sceneCenter = D2D1::Point2F(
        (float)((rect->St().X() + rect->Ed().X()) * 0.5), (float)((rect->St().Y() + rect->Ed().Y()) * 0.5));
    auto translateMatrix =
        D2D1::Matrix3x2F::Translation(sceneCenter.x - windowCenter.x, sceneCenter.y - windowCenter.y);

    _TransformToScene = scaleMatrix * translateMatrix;
    UpdateTransformToWindow();

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

    double scale = GetSceneScale();
    if (scale < Math::Epsilon)
    {
        return;
    }

    auto pt = ToScenePoint(x, y);
    double minDis = NearRadius * scale;
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

    
    _AnchorWindow = {(double)x, (double)y};
    _AnchorSceneTransform = _TransformToScene;

    if (_ROIDrag)
    {
        _DragMode = DragMode::ROI;
        _ROIDrag->DrawMark(true);
        return;
    }
    
    _DragMode = DragMode::Scene;
}

void Display::OnMouseUp(WPARAM btnState, int x, int y)
{
    if (_DragMode == DragMode::ROI)
    {
        _DragMark = 0;
        _ROIDrag->DrawMark(false);
        _ROIDrag = nullptr;
        _OriDragROI.reset(nullptr);
    }

    _DragMode = DragMode::None;
}

void Display::OnMouseMove(WPARAM btnState, int x, int y)
{
    auto anchor = ToScenePoint(_AnchorWindow);
    auto pt = ToScenePoint(x, y);
    if (_DragMode == DragMode::ROI && _ROIDrag)
    {
        _ROIDrag->DragMark(_DragMark, anchor, pt, *_OriDragROI);

        return;
    }

    if (_DragMode == DragMode::Scene)
    {
        // Here is a bit tricky, the behavior we expected is that the translation in the window CS should be applied to the scene transform.
        auto vector = (_AnchorWindow - Point((double)x, (double)y)) * GetSceneScale();
        auto translation = D2D1::Matrix3x2F::Translation((float)vector.X(), (float)vector.Y());
        _TransformToScene = _AnchorSceneTransform * translation;
        UpdateTransformToWindow();

        return;
    }

    assert(_DragMode == DragMode::None);
    {
        double scale = _TransformToScene.m11;
        if (scale < Math::Epsilon)
        {
            return;
        }

        double maxDis = NearRadius * scale;
        bool find = false;
        for (auto&& roi : _ROIs)
        {
            roi->DrawMark(false);
            if (find)
            {
                continue;
            }

            if (!find && (roi->PtNearBoundary(pt, maxDis) ||
                Math::CalcDistanceFormPointToPoint(pt, roi->Center()) < maxDis))
            {
                find = true;
                roi->DrawMark(true);
            }
        }
    }
}

void Display::OnMouseWheel(WPARAM btnState, int x, int y, int delta)
{
    auto anchor = ToScenePoint(x, y);
    if (std::abs((int)(anchor.X() + 0.5) - x) > 5. || std::abs((int)(anchor.Y() + 0.5) - y) > 5.)
    {
        anchor.X(x);
        anchor.Y(y);
    }

    auto pt = _TransformToScene.TransformPoint(D2D1::Point2F((float)anchor.X(), (float)anchor.Y()));
    auto scale = delta > 0 ? 0.9f : 1.1f;
    auto scaleMatrix = D2D1::Matrix3x2F::Scale(scale, scale, pt);

    _TransformToScene = _TransformToScene * scaleMatrix;
    UpdateTransformToWindow();
}

::ShapeViewer::Point Display::ToScenePoint(int x, int y) const
{
    auto pointer = D2D1::Point2F((float)x, (float)y);
    auto pointerInScene = _TransformToScene.TransformPoint(pointer);
    return {pointerInScene.x, pointerInScene.y};
}

::ShapeViewer::Point Display::ToScenePoint(const ::ShapeViewer::Point& pt) const
{
    auto pointer = D2D1::Point2F((float)pt.X(), (float)pt.Y());
    auto pointerInScene = _TransformToScene.TransformPoint(pointer);
    return {pointerInScene.x, pointerInScene.y};
}

void Display::UpdateTransformToWindow()
{
    _TransformToWindow = _TransformToScene;
    _TransformToWindow.Invert();
}

double Display::GetSceneScale() const
{
    return static_cast<double>(_TransformToScene.m11);
}

double Display::GetWindowScale() const
{
    return static_cast<double>(_TransformToWindow.m11);
}

const winrt::com_ptr<ID2D1Factory3>& Display::D2D1Factory() const
{
    return _D2D1Factory;
}
winrt::com_ptr<ID2D1Factory3>& Display::D2D1Factory()
{
    return _D2D1Factory;
}

const winrt::com_ptr<ID2D1RenderTarget>& Display::RenderTarget() const
{
    return _D2D1RenderTarget;
}
winrt::com_ptr<ID2D1RenderTarget>& Display::RenderTarget()
{
    return _D2D1RenderTarget;
}

const winrt::com_ptr<ID2D1SolidColorBrush>& Display::Brush() const
{
    return _Brush;
}
winrt::com_ptr<ID2D1SolidColorBrush>& Display::Brush()
{
    return _Brush;
}
