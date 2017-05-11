
struct Input
{
	float3	pos	: POSITION;
	float3	col	: COLOR;
};

struct V2F
{
	float4	pos	: SV_POSITION;
	float3	col : COLOR;
};

cbuffer CameraConstants : register (b0)
{
	matrix	matView;
	matrix	matProj;
}

BlendState defaultBS;
RasterizerState defaultRS;
DepthStencilState defaultDS;

V2F vs_main(Input input)
{
	V2F output;
	output.pos = mul(mul(float4(input.pos, 1), matView), matProj);
	output.col = input.col;
	return output;
}

float4 ps_main(V2F input) : SV_TARGET
{
	return float4(input.col, 1);
}

technique11
{
	pass
	{
		SetBlendState(defaultBS, float4(0, 0, 0, 0), 0xffffffff);
		SetRasterizerState(defaultRS);
		SetDepthStencilState(defaultDS, 0);
		SetVertexShader(CompileShader(vs_5_0, vs_main()));
		SetPixelShader(CompileShader(ps_5_0, ps_main()));
	}
};