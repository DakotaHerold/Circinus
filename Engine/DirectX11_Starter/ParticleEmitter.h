#pragma once

#include <DirectXMath.h>
#include "Component.h"

class ParticleSystem;

#ifdef HAS_GUI
class GUI;
#endif

class ParticleEmitter : public Component
{
private:
	friend class ParticleSystem;
	friend class RenderingSystem;

	ParticleSystem* ps;
	uint32_t poolIdx;
	uint32_t emitterIdx;

	void SetCBParameters(DirectX::XMFLOAT3 & position, DirectX::XMFLOAT3 & velocity, float lifeTime, float emitRate);

public:

	ParticleEmitter(const std::wstring & particleTexture);

	void SetInitialVelocity(DirectX::XMFLOAT3& velocity) { this->velocity = velocity; }
	void SetInitialVelocity(float x, float y, float z) { velocity = DirectX::XMFLOAT3{ x, y, z }; }
	void SetLifeTime(float lifeTime) { this->lifeTime = lifeTime; }
	void SetEmitRate(float emitRate) { this->emitRate = emitRate; }

	const DirectX::XMFLOAT3& GetInitialVelocity() const { return velocity; }
	float GetLifeTime() const { return lifeTime; }
	float GetEmitRate() const { return emitRate; }

	void Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) override;

private:
#ifdef HAS_GUI
	friend class GUI;
#endif
	std::wstring			texFileName;
	DirectX::XMFLOAT3		velocity;
	float					lifeTime;
	float					emitRate;
};