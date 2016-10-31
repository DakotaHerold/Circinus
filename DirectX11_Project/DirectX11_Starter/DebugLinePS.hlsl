// Defines the input to this pixel shader
// - Should match the output of our corresponding vertex shader

//cbuffer externalData : register(b0)
//{
//	float3 inColor;
//};

struct VertexToPixel
{
	float4 position : SV_POSITION;
};

float4 main(VertexToPixel input) : SV_TARGET
{
	return float4(1.0f, 0.0f, 0.0f, 1.0f);
}