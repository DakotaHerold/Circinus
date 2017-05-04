#include "ParticlePool.h"

void ParticlePool::CleanUp()
{
	bufParticleConstants->Release();
	bufParticles->Release();
	bufDeadList->Release();
	bufDrawList->Release();
	bufParticlesUAV->Release();
	bufParticlesSRV->Release();
	bufDeadListUAV->Release();
	bufDrawListUAV->Release();
	bufDrawListSRV->Release();
	texSRV->Release();
}
