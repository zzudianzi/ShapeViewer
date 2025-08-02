//***************************************************************************************
// color.hlsl by Frank Luna (C) 2015 All Rights Reserved.
//
// Transforms and colors geometry.
//***************************************************************************************

cbuffer cbPerObject : register(b0)
{
	float4x4 gWorldViewProj; 
};

struct VPos
{
	float3 Pos  : POSITION;
};

struct VColor
{
	float4 Color : COLOR;
};

struct VertexOut
{
	float4 Pos  : SV_POSITION;
    float4 Color : COLOR;
};

VertexOut VS(VPos vpos, VColor vcolor)
{
	VertexOut vout;
	
	// Transform to homogeneous clip space.
	vout.Pos = mul(float4(vpos.Pos, 1.0f), gWorldViewProj);
	
	// Just pass vertex color into the pixel shader.
    vout.Color = vcolor.Color;

    return vout;
}

float4 PS(VertexOut pin) : SV_Target
{
    return pin.Color;
}
