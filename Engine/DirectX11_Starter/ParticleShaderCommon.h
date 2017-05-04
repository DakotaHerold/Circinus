#ifndef _SHADER_COMMON_

#define _SHADER_COMMON_

#if defined(__cplusplus)

#pragma once
#include <DirectXMath.h>
typedef DirectX::XMFLOAT2	float2;
typedef DirectX::XMFLOAT3	float3;
typedef DirectX::XMFLOAT4	float4;
typedef uint32_t			uint;
typedef DirectX::XMUINT2	uint2;
typedef DirectX::XMUINT3	uint3;
typedef DirectX::XMUINT4	uint4;

#define CBUFFER				struct
#define REGISTER(x)			/* empty */

#else

#define CBUFFER				cbuffer
#define REGISTER(x)			: register(x)

#endif

#endif