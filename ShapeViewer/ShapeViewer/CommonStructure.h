#pragma once

#include <DirectXMath.h>
#include "MathHelper.h"
#include "Light.h"

struct ObjectConstants
{
    DirectX::XMFLOAT4X4 _WorldViewProj = MathHelper::Identity4x4();
    DirectX::XMFLOAT4 _PulseColor;
    float _Time;
};

struct PassConstants
{
    DirectX::XMFLOAT4X4 _View = MathHelper::Identity4x4();
    DirectX::XMFLOAT4X4 _InvView = MathHelper::Identity4x4();
    DirectX::XMFLOAT4X4 _Proj = MathHelper::Identity4x4();
    DirectX::XMFLOAT4X4 _InvProj = MathHelper::Identity4x4();
    DirectX::XMFLOAT4X4 _ViewProj = MathHelper::Identity4x4();
    DirectX::XMFLOAT4X4 _InvViewProj = MathHelper::Identity4x4();
    DirectX::XMFLOAT3 _EyePosW = {0.f, 0.f, 0.f};
    float _CBPerObjectPad1 = 0.f;
    DirectX::XMFLOAT2 _RenderTargetSize = {0.f, 0.f};
    DirectX::XMFLOAT2 _InvRenderTargetSize = {0.f, 0.f};
    float _NearZ = 0.f;
    float _FarZ = 0.f;
    float _TotalTime = 0.f;
    float _DeltaTime = 0.f;
};

struct MaterialConstants
{
    DirectX::XMFLOAT4 _DiffuseAlbedo = {1.f, 1.f, 1.f, 1.f};
    DirectX::XMFLOAT3 _FresnelR0 = {0.01f, 0.01f, 0.01f};
    float _Roughness = 0.25f;

    DirectX::XMFLOAT4X4 _MatTransform = MathHelper::Identity4x4();
};
