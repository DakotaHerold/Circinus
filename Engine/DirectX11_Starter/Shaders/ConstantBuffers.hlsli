#define MAX_LIGHTS 8
#include "Lights.hlsli"

cbuffer InstanceConstants : register (b0)
{
	matrix	matWorld;
	matrix	matWorld_IT;
}

cbuffer FrameConstants : register (b1)
{
	matrix	matView;
	matrix	matProj;
	float3	camPos;
}

cbuffer LightProperties : register(b2)
{
	float4		EyePosition;            // 16 bytes
										//----------------------------------- (16 byte boundary)
	float4		GlobalAmbient;          // 16 bytes
										//----------------------------------- (16 byte boundary)
	Light		Lights[MAX_LIGHTS];     // 80 * 8 = 640 bytes
};  // Total:                           // 672 bytes (42 * 16 byte boundary)

