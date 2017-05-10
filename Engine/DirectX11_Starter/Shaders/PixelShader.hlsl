#include "Lights.hlsli"

// Struct representing the data we expect to receive from earlier pipeline stages
// - Should match the output of our corresponding vertex shader
// - The name of the struct itself is unimportant
// - The variable names don't have to match other shaders (just the semantics)
// - Each variable must have a semantic, which defines its usage
struct VertexToPixel
{
	// Data type
	//  |
	//  |   Name          Semantic
	//  |    |                |
	//  v    v                v
	float4 position		: SV_POSITION;	// XYZW position (System Value Position)
	float3 normal		: NORMAL;
	float3 worldPos		: POSITION;
	float2 uv			: TEXCOORD;
};

// Variable to fill data from C++ side
cbuffer externalData : register(b0)
{
	DirectionalLight directionalLight; 
	DirectionalLight directionalLight2;
	SpecularLight specularLight;
	PointLight pointLight; 
	float3 camPos; 
};


// --------------------------------------------------------
// The entry point (main method) for our pixel shader
// 
// - Input is the data coming down the pipeline (defined by the struct)
// - Output is a single color (float4)
// - Has a special semantic (SV_TARGET), which means 
//    "put the output of this into the current render target"
// - Named "main" because that's the default the shader compiler looks for
// --------------------------------------------------------
float4 main(VertexToPixel input) : SV_TARGET
{
	// Just return the input color
	// - This color (like most values passing through the rasterizer) is 
	//   interpolated for each pixel between the corresponding vertices 
	//   of the triangle we're rendering

	// Calculate all lights in scene and output 
	// variables needed for calculations 
	float3 pLight1dir = normalize(pointLight.Position - input.worldPos);
	float3 dirToCam = normalize(camPos - input.worldPos); 
	// Do this by summing calculations from helper functions 
	float3 output; 
	input.normal = normalize(input.normal);
	output = calcDirectionalLight(directionalLight, input.normal, 0.75f) +			// Directional Light 
		calcDirectionalLight(directionalLight2, input.normal, 0.75f) + 				// Second Directional Light 
		calcPointLight(pointLight, pLight1dir, input.normal) + 						// Point Light
		calcSpecularLight(specularLight, input.normal, dirToCam, specularLight.LightIntensity, specularLight.SpecularStrength);		// Specular Light 


	return float4(output, 1); 
}