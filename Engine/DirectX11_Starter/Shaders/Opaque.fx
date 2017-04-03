struct Input
{
	float3 position		: POSITION;
	float3 normal		: NORMAL;
	float3 tangent		: TANGENT;
	float2 uv			: TEXCOORD;
};

struct V2F
{
	float4 position		: SV_POSITION;
	float3 normal		: NORMAL;
	float3 tangent		: TANGENT;
	float2 uv			: TEXCOORD;
};

cbuffer InstanceConstants : register (b0)
{
	matrix	matWorld;
	matrix	matWorld_IT;
}

cbuffer FrameConstants : register (b1)
{
	matrix	matView;
	matrix	matProj;
}

Texture2D		texDiffuse	: register(t0);

SamplerState	sampBasic
{
	AddressU = WRAP;
	AddressV = WRAP;
	AddressW = WRAP;
};

V2F vs_main(Input input)
{
	V2F output;

	matrix matMVP = mul(mul(matWorld, matView), matProj);

	output.position = mul(float4(input.position, 1), matMVP);

	output.normal = mul(input.normal, (float3x3)matWorld_IT);
	output.tangent = mul(input.tangent, (float3x3)matWorld_IT);

	output.uv = input.uv;

	return output;
}

float4 ps_main(V2F input) : SV_TARGET
{
	return texDiffuse.Sample(sampBasic, input.uv);
}

technique11
{
	pass
	{
		SetVertexShader(CompileShader(vs_5_0, vs_main()));
		SetPixelShader(CompileShader(ps_5_0, ps_main()));
	}
};