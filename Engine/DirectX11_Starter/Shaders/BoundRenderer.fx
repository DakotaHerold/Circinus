
cbuffer CameraConstants : register (b0)
{
	matrix	matView;
	matrix	matProj;
}

BlendState defaultBS;
RasterizerState defaultRS;
DepthStencilState defaultDS;

float4 vs_main(float3 pos : POSITION) : SV_POSITION
{
	float4 outPos = mul(mul(float4(pos, 1), matView), matProj);
	return outPos;
}

float4 ps_main() : SV_TARGET
{
	return float4(1, 1, 1, 1);
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