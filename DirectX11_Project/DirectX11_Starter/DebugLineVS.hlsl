cbuffer externalData : register(b0)
{
	matrix world; 
	matrix view;
	matrix projection;
};

// Describes individual vertex data
struct VertexShaderInput
{
	float3 position : POSITION;
};

// Defines the output data of our vertex shader
struct VertexToPixel
{
	float4 position : SV_POSITION;
};

VertexToPixel main( VertexShaderInput input ) 
{
	// Set up output
	VertexToPixel output;

	//calculate output position 

	matrix worldViewProj = mul(mul(world, view), projection);
	// Then we convert our 3-component position vector to a 4-component vector
	// and multiply it by our final 4x4 matrix.
	//
	// The result is essentially the position (XY) of the vertex on our 2D 
	// screen and the distance (Z) from the camera (the "depth" of the pixel)
	output.position = mul(float4(input.position, 1.0f), worldViewProj);

	
	// Calculate output position
	/*matrix viewProj = mul(view, projection);
	output.position = mul(float4(input.position, 1.0f), viewProj);*/
	
	
	return output;
}