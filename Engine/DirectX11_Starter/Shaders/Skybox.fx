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

cbuffer FrameConstants : register (b1)
{
	matrix	matView;
	matrix	matProj;
}

TextureCube skyMap : register(t0);
SamplerState linearSampler
{
	AddressU = WRAP;
	AddressV = WRAP;
	AddressW = WRAP;
	Filter = ANISOTROPIC;
	MaxAnisotropy = 16;
};

DepthStencilState ds
{
	DepthFunc = LESS_EQUAL;
};

RasterizerState rs
{
	CullMode = FRONT;
};

VertexToPixel vs_main(VertexShaderInput input)
{
	VertexToPixel output;

	matrix matV = matView;
	matV._41 = 0;
	matV._42 = 0;
	matV._43 = 0;

	//Swizzle such that z == w i.e. z is always 1 i.e. farthest away
	output.position = mul(mul(float4(input.position, 1), matV), matProj).xyww;

	output.uv = input.position;

	return output;
}

float4 ps_main(VertexToPixel input) : SV_TARGET
{
	return skyMap.Sample(linearSampler, input.uv);
}

technique11
{
	pass 
	{
		SetRasterizerState(rs);
		SetDepthStencilState(ds, 0);

		SetVertexShader(CompileShader(vs_5_0, vs_main()));
		SetPixelShader(CompileShader(ps_5_0, ps_main()));
	}
};