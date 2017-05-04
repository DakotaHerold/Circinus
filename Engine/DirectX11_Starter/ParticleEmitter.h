#pragma once

#include <DirectXMath.h>
#include "Component.h"

class ParticleSystem;

class ParticleEmitter : public Component
{
private:
	friend class ParticleSystem;

	ParticleSystem* ps;
	uint32_t poolIdx;
	uint32_t emitterIdx;

public:

	ParticleEmitter(const std::wstring & particleTexture);

	void SetParameters(DirectX::XMFLOAT3 & position, DirectX::XMFLOAT3 & velocity, float lifeTime, float emitRate);
};