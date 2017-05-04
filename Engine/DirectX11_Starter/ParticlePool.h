#pragma once

#include <d3d11.h>
#include <DirectXMath.h>

#include <vector>

#include "Emitter.h"

struct ParticlePool
{
	struct {
		float						deltaTime;
		uint32_t					maxParticles;
		uint32_t					_padding1;
		uint32_t					_padding2;
	}								particleConstants;

	ID3D11Buffer*					bufParticleConstants; 
	ID3D11Buffer*					bufParticles;
	ID3D11Buffer*					bufDeadList;
	ID3D11Buffer*					bufDrawList;
	ID3D11UnorderedAccessView*		bufParticlesUAV;
	ID3D11ShaderResourceView*		bufParticlesSRV;
	ID3D11UnorderedAccessView*		bufDeadListUAV;
	ID3D11UnorderedAccessView*		bufDrawListUAV;
	ID3D11ShaderResourceView*		bufDrawListSRV;
	ID3D11ShaderResourceView*		texSRV;
	bool							particleFirstUpdate;

	std::vector<Emitter>			emitters;

	void CleanUp();
};