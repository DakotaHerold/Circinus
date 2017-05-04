#pragma once

#include <d3d11.h>
#include "SimpleShader.h"
#include "ParticlePool.h"
#include "ParticleEmitter.h"

#include <string>
#include <unordered_map>
#include <vector>

class ParticleSystem
{
private:
	static ParticleSystem* _instance;
public:
	static ParticleSystem* instance() { return _instance; }

public:
	ParticleSystem()
		:
		device(nullptr),
		context(nullptr),
		particleVS(nullptr),
		particlePS(nullptr),
		particleCS(nullptr),
		particleEmitterCS(nullptr)
	{}

	bool Init(ID3D11Device* device, ID3D11DeviceContext* context);

	void Update(float deltaTime, float totalTime);

	bool Draw(const DirectX::XMFLOAT4X4& matView, const DirectX::XMFLOAT4X4& matProj);

	void CleanUp();

	bool CreateParticleEmitter(const std::wstring& particleTexture, ParticleEmitter* emitterComponent);

private:
	friend class ParticleEmitter;

	bool CreateParticlePool(uint32_t maxParticles, const std::wstring& texFileName);

private:
	ID3D11Device*					device;
	ID3D11DeviceContext*			context;

	SimpleVertexShader*				particleVS;
	SimplePixelShader*				particlePS;
	SimpleComputeShader*			particleInitCS;
	SimpleComputeShader*			particleEmitterCS;
	SimpleComputeShader*			particleCS;

	ID3D11Buffer*					bufEmitter;

	ID3D11Buffer*					bufQuadIndices;
	ID3D11Buffer*					bufIndirectDrawArgs;

	ID3D11SamplerState*				sampler;
	ID3D11BlendState*				blendState;
	ID3D11DepthStencilState*		depthStencilState;

	typedef std::unordered_map<std::wstring, uint32_t> map_t;
	map_t							poolMap;

	std::vector<ParticlePool>		pools;

	uint32_t						totalEmitCount;
};