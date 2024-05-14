#include "pch.h"
#include "VisText.h"
#include "Display.h"

using namespace ShapeViewer;

VisText::VisText(const std::wstring& text) : _Text(text), _Position(Position::TopLeft), _CS(CoordinateSystem::Window)
{
}

const std::wstring& VisText::Text() const
{
    return _Text;
}

std::wstring& VisText::Text()
{
    return _Text;
}

VisText::Position VisText::GetPosition() const
{
    return _Position;
}

void VisText::SetPosition(Position value)
{
    _Position = value;
}

VisText::CoordinateSystem VisText::GetCoordinateSystem() const
{
    return _CS;
}

void VisText::SetCoordinateSystem(CoordinateSystem value)
{
    _CS = value;
}

void VisText::Draw()
{
    if (!_Display)
    {
        return;
    }

    auto writeFactory = _Display->DWriteFactory();
    auto textFormat = _Display->DWriteTextFormat();
    auto renderTarget = _Display->RenderTarget();
    D2D1_SIZE_F size = renderTarget->GetSize();
    _TextLayout = nullptr;
    writeFactory->CreateTextLayout(_Text.c_str(), (UINT32)_Text.size(), textFormat.get(), size.width, size.height, _TextLayout.put());

    DWRITE_OVERHANG_METRICS overhang;
    _TextLayout->GetOverhangMetrics(&overhang);

    float minW = 8.;
    winrt::check_hresult(_TextLayout->DetermineMinWidth(&minW));
    DWRITE_TEXT_METRICS metrics;
    winrt::check_hresult(_TextLayout->GetMetrics(&metrics));

    _TextLayout->SetMaxWidth(metrics.width);
    _TextLayout->SetMaxHeight(metrics.height);
    
    auto brush = _Display->Brush();
    brush->SetColor(D2D1::ColorF(D2D1::ColorF::White));
    renderTarget->DrawTextLayout(D2D1::Point2F(0.f, 0.f), _TextLayout.get(), brush.get());
}

std::optional<::ShapeViewer::Rect> VisText::BoundingRect() const
{
    throw;
}