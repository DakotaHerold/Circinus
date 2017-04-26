// Light Structs 
#ifndef _LIGHTS_HLSLI
#define _LIGHTS_HLSLI
#define MAX_LIGHTS 8
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
	int2        Padding;                // 8 bytes
										//----------------------------------- (16 byte boundary)
};  // Total:                           // 80 bytes (5 * 16 byte boundary)

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

	return light.Color * pow(NdotH, 128/*Material.SpecularPower*/);
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

LightingResult ComputeLighting(float4 P, float3 N, float4 EyePosition, Light Lights[MAX_LIGHTS])
{
	float3 V = normalize(EyePosition - P).xyz;

	LightingResult totalResult = { { 0, 0, 0, 0 },{ 0, 0, 0, 0 } };

	[unroll]
	for (int i = 0; i < MAX_LIGHTS; ++i)
	{
		LightingResult result = { { 0, 0, 0, 0 },{ 0, 0, 0, 0 } };

		if (!Lights[i].Enabled) continue;

		switch (Lights[i].LightType)
		{
		case 0:
		{
			result = DoDirectionalLight(Lights[i], V, P, N);
		}
		break;
		case 1:
		{
			result = DoPointLight(Lights[i], V, P, N);
		}
		break;
		case 2:
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
//
//struct Lighting
//{
//	float4 Ambient;
//	float4 Diffuse;
//	float4 Specular;
//};
//
//struct DirectionalLight
//{
//	float4 AmbientColor;
//	float4 DiffuseColor;
//	float3 Direction;
//};
//
//struct PointLight
//{
//	float3 position;
//	float4 diffuseColor;
//	float  diffusePower;
//	float4 specularColor;
//	float  specularPower;
//};
//
////struct SpecularLight
////{
////	float4 SpecularColor;
////	float3 Direction;
////	float SpecularStrength;
////	float LightIntensity;
////};
//
//// Helper Functions 
//
//// Directional Light 
//Lighting calcDirectionalLight(DirectionalLight light, float3 normal, float strength)
//{
//	Lighting OUT;
//	OUT.Ambient = float4(0, 0, 0, 0);
//	OUT.Diffuse = float4(0, 0, 0, 0);
//	OUT.Specular = float4(0, 0, 0, 0);
//	// Direction of DirectionalLight 
//	float3 dir = normalize(-light.Direction);
//	// not sure if needed, normalize to be safe 
//	normal = normalize(normal);
//	float NdotL = saturate(dot(normal, dir));
//	OUT.Diffuse = light.DiffuseColor * NdotL * strength;
//	OUT.Ambient = light.AmbientColor;
//	return OUT;
//}
//
//Lighting calcPointLight(PointLight light, float3 normal, float3 dir, float3 dirToCamera)
//{
//	Lighting OUT;
//	OUT.Ambient = float4(0, 0, 0, 0);
//	OUT.Diffuse = float4(0, 0, 0, 0);
//	OUT.Specular = float4(0, 0, 0, 0);
//
//	// not sure if needed, normalize to be safe 
//	normal = normalize(normal);
//	float NdotL = saturate(dot(normal, dir));
//	OUT.Diffuse = NdotL * light.diffuseColor * light.diffusePower;
//
//	// Blinn-Phong lighting
//	// Calculate “half-way” vector between the light vector
//	// and view vector (direction from surface to camera)
//	float3 h = normalize(dir + dirToCamera);
//
//	float NdotH = saturate(dot(normal, h));
//	// Raise the ratio to a power equal to some
//	float4 specAmt = pow(NdotH, pow(2, light.specularPower));
//
//	OUT.Specular = specAmt * light.specularColor;
//	return OUT;
//}
//
////float3 calcSpecularLight(SpecularLight light, float3 normal, float3 viewDir, float intensity, float strength)
////{
////	// Credit to http://www.rastertek.com/dx11tut10.html for reference material 
////
////	// not sure if needed, normalize to be safe 
////	normal = normalize(normal);
////	float3 dir = normalize(-light.Direction);
////	// Assumes that strength will be greater than 0 
////	float3 reflection = normalize(2 * intensity * normal - dir);
////	float4 specular = pow(saturate(dot(reflection, viewDir)), strength);
////	return specular.xyz * light.SpecularColor.xyz;
////}
