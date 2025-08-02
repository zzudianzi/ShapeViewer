//***************************************************************************************
// color.hlsl by Frank Luna (C) 2015 All Rights Reserved.
//
// Transforms and colors geometry.
//***************************************************************************************

cbuffer cbPerObject : register(b0)
{
	float4x4 gWorldViewProj; 
};

struct VertexIn
{
	float3 Pos  : POSITION;
	float3 Tangent  : TANGENT;
	float3 Normal  : NORMAL;
	float2 Tex0  : TEXCOORDONE;
	float2 Tex1  : TEXCOORDTWO;
    float4 Color : COLOR;
};

struct VertexOut
{
	float4 Pos  : SV_POSITION;
    float4 Color : COLOR;
};

VertexOut VS(VertexIn vin)
{
	VertexOut vout;
	
	// Transform to homogeneous clip space.
	vout.Pos = mul(float4(vin.Pos, 1.0f), gWorldViewProj);
	
	// Just pass vertex color into the pixel shader.
    vout.Color = vin.Color;
    
    return vout;
}

float4 PS(VertexOut pin) : SV_Target
{
    return pin.Color;
}
