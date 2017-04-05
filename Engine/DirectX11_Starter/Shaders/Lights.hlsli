// Light Structs 
struct DirectionalLight
{
	float4 AmbientColor;
	float4 DiffuseColor;
	float3 Direction;
};

struct PointLight
{
	float4 PointLightColor;
	float3 Position;
};

struct SpecularLight
{
	float4 SpecularColor;
	float3 Direction;
	float SpecularStrength;
	float LightIntensity;
};

// Helper Functions 

// Directional Light 
float3 calcDirectionalLight(DirectionalLight light, float3 normal, float strength)
{
	// Direction of DirectionalLight 
	float3 dir = normalize(-light.Direction);
	// not sure if needed, normalize to be safe 
	normal = normalize(normal);
	float NdotL = saturate(dot(normal, dir));
	float3 output = light.DiffuseColor.xyz * NdotL * strength;
	output += light.AmbientColor.xyz;
	return output;
}

float3 calcPointLight(PointLight light, float3 dir, float3 normal)
{
	// not sure if needed, normalize to be safe 
	normal = normalize(normal);
	float NdotL = saturate(dot(normal, dir));
	return NdotL * light.PointLightColor.xyz;
}

float3 calcSpecularLight(SpecularLight light, float3 normal, float3 viewDir, float intensity, float strength)
{
	// Credit to http://www.rastertek.com/dx11tut10.html for reference material 

	// not sure if needed, normalize to be safe 
	normal = normalize(normal);
	float3 dir = normalize(-light.Direction);
	// Assumes that strength will be greater than 0 
	float3 reflection = normalize(2 * intensity * normal - dir);
	float4 specular = pow(saturate(dot(reflection, viewDir)), strength);
	return specular.xyz * light.SpecularColor.xyz;
}
