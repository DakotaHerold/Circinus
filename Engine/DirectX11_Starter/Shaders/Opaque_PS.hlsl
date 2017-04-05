#include "ConstantBuffers.hlsli"
#include "CommonStructs.hlsli"

Texture2D		texDiffuse	: register(t0);

SamplerState	sampBasic	: register(s0);


float4 main(V2F input) : SV_TARGET
{
	return texDiffuse.Sample(sampBasic, input.uv);
}