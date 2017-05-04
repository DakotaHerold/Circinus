#include "Particle.h"

RWStructuredBuffer<Particle> particles : register(u0);
RWStructuredBuffer<uint> deadList : register(u1);

[numthreads(1024, 1, 1)]
void main( uint3 DTid : SV_DispatchThreadID )
{
	particles[DTid.x].position = float4(0, 0, 0, 0);
	particles[DTid.x].velocity = float4(0, 0, 0, 0);
	deadList[DTid.x] = DTid.x;
}