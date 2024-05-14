#pragma once

#include "Rect.h"

#include "ROI.h"

namespace ShapeViewer
{
class Overlay;
class Display
{
  public:
    Display();
    ~Display();

    enum class DragMode
    {
        None,
        ROI,
        Scene,
    };

    void SetSceneRect(const Rect& rc);

    bool FitSize();

    // Overlay
    Overlay* CreateOverlay(bool scaling = true);
    bool DeleteOverlay(Overlay* overlay);
    bool DeleteOverlay(int index);
    void DeleteAllOverlays();
    int GetOverlayCount() const;
    Overlay* GetOverlay(int index);

    // ROI
    int AddROI(ROI* roi);
    bool DeleteROI(ROI* roi);
    bool DeleteROI(int index);
    bool DeleteAllROIs();
    int GetROICount() const;

    const D2D1::Matrix3x2F& TransformToScene() const;
    D2D1::Matrix3x2F& TransformToScene();
    void TransformToScene(const D2D1::Matrix3x2F& value);

    const D2D1::Matrix3x2F& TransformToWindow() const;
    D2D1::Matrix3x2F& TransformToWindow();
    void TransformToWindow(const D2D1::Matrix3x2F& value);

    void Draw();

    virtual void OnMouseDown(WPARAM btnState, int x, int y);
    virtual void OnMouseUp(WPARAM btnState, int x, int y);
    virtual void OnMouseMove(WPARAM btnState, int x, int y);
    virtual void OnMouseWheel(WPARAM btnState, int x, int y, int delta);

    const winrt::com_ptr<ID2D1Factory3>& D2D1Factory() const;
    winrt::com_ptr<ID2D1Factory3>& D2D1Factory();
    const winrt::com_ptr<IDWriteFactory>& DWriteFactory() const;
    winrt::com_ptr<IDWriteFactory>& DWriteFactory();
    const winrt::com_ptr<IDWriteTextFormat>& DWriteTextFormat() const;
    winrt::com_ptr<IDWriteTextFormat>& DWriteTextFormat();
    const winrt::com_ptr<ID2D1RenderTarget>& RenderTarget() const;
    winrt::com_ptr<ID2D1RenderTarget>& RenderTarget();
    const winrt::com_ptr<ID2D1SolidColorBrush>& Brush() const;
    winrt::com_ptr<ID2D1SolidColorBrush>& Brush();

    ::ShapeViewer::Point ToScenePoint(int x, int y) const;
    ::ShapeViewer::Point ToScenePoint(const ::ShapeViewer::Point& pt) const;

    static constexpr double NearRadius = 5.;

  private:
    void UpdateTransformToWindow();
    double GetSceneScale() const;
    double GetWindowScale() const;

  private:
    std::vector<Overlay*> _Overlays;

    D2D1::Matrix3x2F _TransformToScene;
    D2D1::Matrix3x2F _TransformToWindow;

    std::vector<ROI*> _ROIs;
    ::ShapeViewer::Point _AnchorWindow;
    D2D1::Matrix3x2F _AnchorSceneTransform;
    DragMode _DragMode = DragMode::None;
    int _DragMark;
    ROI* _ROIDrag = nullptr;
    std::unique_ptr<ROI> _OriDragROI;

    // D2D
    winrt::com_ptr<ID2D1Factory3> _D2D1Factory;
    winrt::com_ptr<ID2D1RenderTarget> _D2D1RenderTarget;
    winrt::com_ptr<IDWriteFactory> _DWriteFactory;
    winrt::com_ptr<IDWriteTextFormat> _TextFormat;
    winrt::com_ptr<ID2D1SolidColorBrush> _Brush;
};
} // namespace ShapeViewer
