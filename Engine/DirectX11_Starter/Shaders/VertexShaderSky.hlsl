cbuffer externalData : register(b0)
{
	matrix matWorld;
	matrix matView;
	matrix matProj;
};

struct VertexShaderInput
{
	float3 position		: POSITION;     // XYZ position
	float3 normal		: NORMAL;
	float2 uv			: TEXCOORD;
};

struct VertexToPixel
{
	float4 position		: SV_POSITION;	// XYZW position (System Value Position)
	float3 uv			: TEXCOORD;
};

VertexToPixel main(VertexShaderInput input)
{
	VertexToPixel output;

	//Swizzle such that z == w i.e. z is always 1 i.e. farthest away
	output.position = mul(mul(mul(input.position, matWorld), matView), matProj).xyww;

	output.uv = input.position;

	return output;
}