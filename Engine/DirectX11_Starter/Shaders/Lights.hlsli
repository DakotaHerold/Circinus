//Reference: https://www.3dgep.com/texturing-lighting-directx-11/
// Light Structs 
#ifndef _LIGHTS_HLSLI
#define _LIGHTS_HLSLI
#define MAX_LIGHTS 8
#define DIRECTIONAL_LIGHT 0
#define POINT_LIGHT 1
#define SPOT_LIGHT 2

struct Light
{
	float4      Position;               // 16 bytes
										//----------------------------------- (16 byte boundary)
	float4      Direction;              // 16 bytes
										//----------------------------------- (16 byte boundary)
	float4      Color;                  // 16 bytes
										//----------------------------------- (16 byte boundary)
	float       SpotAngle;              // 4 bytes
	float       ConstantAttenuation;    // 4 bytes
	float       LinearAttenuation;      // 4 bytes
	float       QuadraticAttenuation;   // 4 bytes
										//----------------------------------- (16 byte boundary)
	int         LightType;              // 4 bytes
	bool        Enabled;                // 4 bytes
	int			SpecularAmount;			// 4 bytes
	int         Padding;                // 4 bytes
										//----------------------------------- (16 byte boundary)
};  // Total:                           // 80 bytes (5 * 16 byte boundary)

cbuffer LightProperties : register(b2)
{
	float4		EyePosition;            // 16 bytes
										//----------------------------------- (16 byte boundary)
	float4		GlobalAmbient;          // 16 bytes
										//----------------------------------- (16 byte boundary)
	int			Count;					// 4 bytes
	int3		Padding;				// 12 bytes
										//----------------------------------- (16 byte boundary)
	Light		Lights[MAX_LIGHTS];     // 80 * 8 = 640 bytes
};  // Total:                           // 688 bytes (43 * 16 byte boundary)

float4 DoDiffuse(Light light, float3 L, float3 N)
{
	float NdotL = max(0, dot(N, L));
	return light.Color * NdotL;
}

float4 DoSpecular(Light light, float3 V, float3 L, float3 N)
{
	// Blinn-Phong lighting
	float3 H = normalize(L + V);
	float NdotH = max(0, dot(N, H));

	return light.Color * pow(NdotH, pow(2, light.SpecularAmount));
}

float DoAttenuation(Light light, float d)
{
	return 1.0f / (light.ConstantAttenuation + light.LinearAttenuation * d + light.QuadraticAttenuation * d * d);
}

struct LightingResult
{
	float4 Diffuse;
	float4 Specular;
};

LightingResult DoPointLight(Light light, float3 V, float4 P, float3 N)
{
	LightingResult result;

	float3 L = (light.Position - P).xyz;
	float distance = length(L);
	L = L / distance;

	float attenuation = DoAttenuation(light, distance);

	result.Diffuse = DoDiffuse(light, L, N) * attenuation;
	result.Specular = DoSpecular(light, V, L, N) * attenuation;

	return result;
}

LightingResult DoDirectionalLight(Light light, float3 V, float4 P, float3 N)
{
	LightingResult result;

	float3 L = -light.Direction.xyz;

	result.Diffuse = DoDiffuse(light, L, N);
	result.Specular = DoSpecular(light, V, L, N);

	return result;
}

float DoSpotCone(Light light, float3 L)
{
	float spotMinAngle = cos(light.SpotAngle);
	float spotMaxAngle = (spotMinAngle + 1.0f) / 2.0f;
	float cosAngle = dot(light.Direction.xyz, L);
	return smoothstep(spotMinAngle, spotMaxAngle, cosAngle);
}

LightingResult DoSpotLight(Light light, float3 V, float4 P, float3 N)
{
	LightingResult result;

	float3 L = (light.Position - P).xyz;
	float distance = length(L);
	L = L / distance;

	float attenuation = DoAttenuation(light, distance);
	float spotIntensity = DoSpotCone(light, -L);

	result.Diffuse = DoDiffuse(light, L, N) * attenuation * spotIntensity;
	result.Specular = DoSpecular(light, V, L, N) * attenuation * spotIntensity;

	return result;
}

LightingResult ComputeLighting(float4 P, float3 N)
{
	// Direction to cam
	float3 V = normalize(EyePosition - P).xyz;

	LightingResult totalResult = { { 0, 0, 0, 0 },{ 0, 0, 0, 0 } };

	[unroll]
	for (int i = 0; i < Count; ++i)
	{
		LightingResult result = { { 0, 0, 0, 0 },{ 0, 0, 0, 0 } };

		if (!Lights[i].Enabled) continue;

		switch (Lights[i].LightType)
		{
		case DIRECTIONAL_LIGHT:
		{
			result = DoDirectionalLight(Lights[i], V, P, N);
		}
		break;
		case POINT_LIGHT:
		{
			result = DoPointLight(Lights[i], V, P, N);
		}
		break;
		case SPOT_LIGHT:
		{
			result = DoSpotLight(Lights[i], V, P, N);
		}
		break;
		}
		totalResult.Diffuse += result.Diffuse;
		totalResult.Specular += result.Specular;
	}

	totalResult.Diffuse = saturate(totalResult.Diffuse);
	totalResult.Specular = saturate(totalResult.Specular);

	return totalResult;
}
#endif