#include "pch.h"

#include "JsonHelper.h"

#include <winrt/Windows.Storage.h>
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Data.Json.h>

#include <DirectXMath.h>
#include <DirectXColors.h>

using namespace DirectX;
using namespace ShapeViewer;
using namespace winrt;
using namespace Windows::Data::Json;
using namespace Windows::Foundation;
using namespace Windows::Storage;

ShapeViewer::Polyline JsonHelper::ReadJson(const std::wstring& jsonString)
{
    auto jsonValue = JsonValue::Parse(jsonString);

    auto array = jsonValue.GetArray();

    auto elementNum = array.Size();

    Polyline polyline;
    Polyline::Vertices& vertices = polyline.Points();
    vertices.resize(elementNum);

    for (int i = 0; i < elementNum; i++)
    {
        JsonObject vertice = array.GetAt(i).GetObjectW();
        vertices[i]._Pos = {
            static_cast<float>(vertice.GetNamedNumber(L"x")),
            static_cast<float>(vertice.GetNamedNumber(L"y")),
            static_cast<float>(vertice.GetNamedNumber(L"z"))};
        vertices[i]._Color = XMFLOAT4(DirectX::Colors::ForestGreen);
    }

    return polyline;
}