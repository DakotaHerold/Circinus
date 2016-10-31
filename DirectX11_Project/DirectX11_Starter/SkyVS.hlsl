// Implemented by Chris Cascioli 
// Constant buffer for C++ data being passed in
cbuffer externalData : register(b0)
{
	matrix view;
	matrix projection;
};

// Describes individual vertex data
struct VertexShaderInput
{
	float3 position		: POSITION;
	float2 uv			: TEXCOORD;
	float3 normal		: NORMAL;
	float3 tangent		: TANGENT;
};

// Defines the output data of our vertex shader
struct VertexToPixel
{
	float4 position		: SV_POSITION;
	float3 uvw          : TEXCOORD0;
};

// The entry point for our vertex shader
VertexToPixel main(VertexShaderInput input)
{
	// Set up output
	VertexToPixel output;

	// Make a copy of the view matrix
	matrix viewNoMovement = view;
	viewNoMovement._41 = 0;
	viewNoMovement._42 = 0;
	viewNoMovement._43 = 0;

	// Handle my VP
	matrix viewProj = mul(viewNoMovement, projection);
	output.position = mul(float4(input.position, 1.0f), viewProj);

	// Ensure the depth is 1.0
	output.position.z = output.position.w;

	// Use the raw input position as my sample direction
	output.uvw = input.position;

	return output;
}