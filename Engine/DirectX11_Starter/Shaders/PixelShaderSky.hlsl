struct VertexToPixel
{
	float4 position		: SV_POSITION;	// XYZW position (System Value Position)
	float3 uv			: TEXCOORD;
};

TextureCube skyMap : register(t0);
SamplerState linearSampler : register(s0);

float4 main(VertexToPixel input) : SV_TARGET
{
	return skyMap.Sample(linearSampler, input.uv);
}