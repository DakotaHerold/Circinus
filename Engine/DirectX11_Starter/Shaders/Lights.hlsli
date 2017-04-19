// Light Structs 

struct Lighting
{
	float4 Ambient;
	float4 Diffuse;
	float4 Specular;
};

struct DirectionalLight
{
	float4 AmbientColor;
	float4 DiffuseColor;
	float3 Direction;
};

struct PointLight
{
	float3 position;
	float4 diffuseColor;
	float  diffusePower;
	float4 specularColor;
	float  specularPower;
};

//struct SpecularLight
//{
//	float4 SpecularColor;
//	float3 Direction;
//	float SpecularStrength;
//	float LightIntensity;
//};

// Helper Functions 

// Directional Light 
Lighting calcDirectionalLight(DirectionalLight light, float3 normal, float strength)
{
	Lighting OUT;
	OUT.Ambient = float4(0, 0, 0, 0);
	OUT.Diffuse = float4(0, 0, 0, 0);
	OUT.Specular = float4(0, 0, 0, 0);
	// Direction of DirectionalLight 
	float3 dir = normalize(-light.Direction);
	// not sure if needed, normalize to be safe 
	normal = normalize(normal);
	float NdotL = saturate(dot(normal, dir));
	OUT.Diffuse = light.DiffuseColor * NdotL * strength;
	OUT.Ambient = light.AmbientColor;
	return OUT;
}

Lighting calcPointLight(PointLight light, float3 normal, float3 dir, float3 dirToCamera)
{
	Lighting OUT;
	OUT.Ambient = float4(0, 0, 0, 0);
	OUT.Diffuse = float4(0, 0, 0, 0);
	OUT.Specular = float4(0, 0, 0, 0);
	// not sure if needed, normalize to be safe 
	normal = normalize(normal);
	float NdotL = saturate(dot(normal, dir));
	OUT.Diffuse = NdotL * light.diffuseColor * light.diffusePower;

	// Calculate “half-way” vector between the light vector
	// and view vector (direction from surface to camera)
	float3 h = normalize(dir + dirToCamera);
	// Calculate the cosine of the normal and halfway vector
	// Saturate() ensures it’s between 0 and 1
	float NdotH = saturate(dot(normal, h));
	// Raise the ratio to a power equal to some
	// “shininess” factor – try a number between 1 and 64
	float4 specAmt = pow(NdotH, light.specularPower);

	OUT.Specular = specAmt * light.specularColor;
	return OUT;
}

//float3 calcSpecularLight(SpecularLight light, float3 normal, float3 viewDir, float intensity, float strength)
//{
//	// Credit to http://www.rastertek.com/dx11tut10.html for reference material 
//
//	// not sure if needed, normalize to be safe 
//	normal = normalize(normal);
//	float3 dir = normalize(-light.Direction);
//	// Assumes that strength will be greater than 0 
//	float3 reflection = normalize(2 * intensity * normal - dir);
//	float4 specular = pow(saturate(dot(reflection, viewDir)), strength);
//	return specular.xyz * light.SpecularColor.xyz;
//}
