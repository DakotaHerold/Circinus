#include "../Particle.h"
#include "../Emitter.h"
#include "Noise.hlsli"

RWStructuredBuffer<Particle> particles : register(u0);

ConsumeStructuredBuffer<uint> deadList : register(u1);

[numthreads(1024, 1, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
	if (DTid.x < deadParticles && DTid.x < emitCount)
	{
		uint pid = deadList.Consume();

		particles[pid].position = position;

		float3 randomFloat = curlNoise3D(particles[pid].position.xyz, totalTime);
		particles[pid].position.xz += (randomFloat.xz % 10) / 100;

		particles[pid].velocity = velocity;

		float3 randomFloat2 = curlNoise3D(particles[pid].velocity.xyz, totalTime);
		particles[pid].velocity.xyz += (randomFloat2.xyz % 10) / 100;
	}
}
