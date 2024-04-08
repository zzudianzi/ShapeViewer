#pragma once

#include <DirectXMath.h>

class Light
{
  public:
    DirectX::XMFLOAT3 _Strength;  // Light color
    float _FallOffStart;          // Point/Spot light only
    DirectX::XMFLOAT3 _Direction; // Directional/Spot light only
    float _FallOffEnd;            // Point/Spot light only
    DirectX::XMFLOAT3 _Position;  // Point/Spot light only
    float _SpotPower;             // Spot light only
};
