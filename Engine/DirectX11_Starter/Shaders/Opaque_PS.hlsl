#include "ConstantBuffers.hlsli"
#include "CommonStructs.hlsli"
#include "Lights.hlsli"

Texture2D		texDiffuse	: register(t0);

SamplerState	sampBasic	: register(s0);


float4 main(V2F input) : SV_TARGET
{
	//DirectionalLight dL;
	//dL.AmbientColor = float4(0.2f, 0.2f, 0.2f, 1.0f);
	//dL.DiffuseColor = float4(1.0f, 1.0f, 1.0f, 1.0f);
	//dL.Direction = float3(0, 1.0f, 0);

	float4 totalLight = float4(/*calcDirectionalLight(dL, input.normal, 1.0f), 1) * texDiffuse.Sample(sampBasic, input.uv*/0,0,0,0);

	return totalLight;
}