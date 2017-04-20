#include "Lights.hlsli"

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
	float3 worldPos		: POSITION;
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
	float3	camPos;
}

Texture2D		texDiffuse	: register(t0);

SamplerState	sampBasic
{
	AddressU = WRAP;
	AddressV = WRAP;
	AddressW = WRAP;
};

DepthStencilState defaultDS;
RasterizerState defaultRS;

V2F vs_main(Input input)
{
	V2F output;

	matrix matMVP = mul(mul(matWorld, matView), matProj);

	output.position = mul(float4(input.position, 1), matMVP);

	output.normal = mul(input.normal, (float3x3)matWorld_IT);
	output.tangent = mul(input.tangent, (float3x3)matWorld_IT);

	output.worldPos = mul(float4(input.position, 1.0f), matWorld_IT).xyz;

	output.uv = input.uv;

	return output;
}

float4 ps_main(V2F input) : SV_TARGET
{
	DirectionalLight dL;
	dL.AmbientColor = float4(0.4f, 0.4f, 0.4f, 1.0f);
	dL.DiffuseColor = float4(1.0f, 1.0f, 1.0f, 1.0f);
	dL.Direction = float3(0, -1.0f, 0);

	PointLight pL;
	pL.position = float3(0, -0.5f, 0);
	pL.diffuseColor = float4(1, 0, 0, 1);
	pL.diffusePower = 1;
	pL.specularColor = float4(1, 1, 1, 1);
	pL.specularPower = 8;

	float3 pLight1dir = normalize(input.worldPos - pL.position);
	float3 dirToCam = normalize(camPos - input.worldPos);

	Lighting lightOut = calcDirectionalLight(dL, input.normal, 1.0f) /*+ calcDirectionalLight(dL2, input.normal, 1.0f) + calcDirectionalLight(dL3, input.normal, 1.0f) + calcDirectionalLight(dL4, input.normal, 1.0f)*/;
	Lighting pLight = calcPointLight(pL, input.normal, pLight1dir, dirToCam);
	
	return ((lightOut.Ambient + lightOut.Diffuse) + (pLight.Ambient + pLight.Diffuse)) * texDiffuse.Sample(sampBasic, input.uv) + (lightOut.Specular + pLight.Specular);
}

technique11
{
	pass
	{
		SetRasterizerState(defaultRS);
		SetDepthStencilState(defaultDS, 0);
		SetVertexShader(CompileShader(vs_5_0, vs_main()));
		SetPixelShader(CompileShader(ps_5_0, ps_main()));
	}
};