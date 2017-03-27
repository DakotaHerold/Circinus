Texture2D		texDiffuse	: register(t0);

SamplerState	sampBasic	: register(s0);

struct V2F
{
	float4 position		: SV_POSITION;
	float3 normal		: NORMAL;
	float3 tangent		: TANGENT;
	float2 uv			: TEXCOORD;
};

float4 main(V2F input) : SV_TARGET
{
	return texDiffuse.Sample(sampBasic, input.uv);
}