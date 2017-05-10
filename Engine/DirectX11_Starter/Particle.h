#ifndef _PARTICLE_
#define _PARTICLE_

#include "ParticleShaderCommon.h"

struct Particle
{
	float4		position;	// w = age
	float4		velocity;	// w = life time
};

#endif 
