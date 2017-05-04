struct V2F
{
	float4 position : SV_POSITION;
	float2 texcoord : TEXCOORD0;
};

Texture2D tex : register(t0);
SamplerState samp : register(s0);

float4 main(V2F input) : SV_TARGET
{
	float4 col = tex.Sample(samp, input.texcoord);
	return col;// float4(1, 0, 0, 1);// float4(val, 0, 0, 1.0f);
}