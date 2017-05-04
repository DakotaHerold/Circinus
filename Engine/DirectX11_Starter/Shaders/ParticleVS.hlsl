struct Particle
{
	float4 position;  // w = age
	float4 velocity;  // w = life time;
};

StructuredBuffer<Particle> particles : register(t0);

StructuredBuffer<uint> drawList : register(t1);


cbuffer CameraConstants : register(b0)
{
	matrix view;
	matrix projection;
};

struct V2F
{
	float4 position : SV_POSITION;
	float4 texcoord : TEXCOORD0;
};

V2F main(uint vid : SV_VertexID, uint iid : SV_InstanceID)
{
	V2F output;

	uint pid = drawList[iid];
	float4 pos = particles[pid].position;
	pos.w = 1;

	pos = mul(pos, view);

	float2 uv = float2(vid % 2, vid / 2);
	pos.xy += (uv - 0.5) * 1;

	output.position = mul(pos, projection);
	output.texcoord.xy = uv;

	output.texcoord.w = 1 - (particles[pid].position.w / particles[pid].velocity.w);

	return output;
}