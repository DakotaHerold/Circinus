#include "ParticleSystem.h"
#include "Particle.h"
#include "Emitter.h"

#include <WICTextureLoader.h>

#include "FrameCapture.h"

ParticleSystem* ParticleSystem::_instance = nullptr;

bool ParticleSystem::Init(ID3D11Device* device, ID3D11DeviceContext* context)
{
	if (nullptr != _instance)
		return false;

	HRESULT hr = S_OK;

	particleInitCS = new SimpleComputeShader(device, context);
	assert(particleInitCS->LoadShaderFile(L"Assets/ShaderObjs/ParticleInitCS.cso"));

	particleCS = new SimpleComputeShader(device, context);
	assert(particleCS->LoadShaderFile(L"Assets/ShaderObjs/ParticleCS.cso"));

	particleEmitterCS = new SimpleComputeShader(device, context);
	assert(particleEmitterCS->LoadShaderFile(L"Assets/ShaderObjs/ParticleEmitterCS.cso"));

	particleVS = new SimpleVertexShader(device, context);
	assert(particleVS->LoadShaderFile(L"Assets/ShaderObjs/ParticleVS.cso"));

	particlePS = new SimplePixelShader(device, context);
	assert(particlePS->LoadShaderFile(L"Assets/ShaderObjs/ParticlePS.cso"));

	{
		CD3D11_BUFFER_DESC indicesDesc(
			sizeof(uint32_t) * 6,
			D3D11_BIND_INDEX_BUFFER,
			D3D11_USAGE_IMMUTABLE
		);

		uint32_t indices[] = { 0, 2, 3, 0, 3, 1 };

		D3D11_SUBRESOURCE_DATA data = {};
		data.pSysMem = indices;

		hr = device->CreateBuffer(&indicesDesc, &data, &bufQuadIndices);
		assert(hr == S_OK);
	}

	{
		CD3D11_BUFFER_DESC indDrawDesc(
			sizeof(uint32_t) * 8, // 5, but align with 16 bytes
			D3D11_BIND_UNORDERED_ACCESS,
			D3D11_USAGE_DEFAULT,
			0,
			D3D11_RESOURCE_MISC_DRAWINDIRECT_ARGS
		);

		uint32_t args[] = { 6, 0, 0, 0, 0, 0, 0, 0 };

		D3D11_SUBRESOURCE_DATA data = {};
		data.pSysMem = args;

		hr = device->CreateBuffer(&indDrawDesc, &data, &bufIndirectDrawArgs);
		assert(hr == S_OK);
	}

	{
		auto info = particleEmitterCS->GetBufferInfo("Emitter");
		bufEmitter = info->ConstantBuffer;
	}

	{
		D3D11_SAMPLER_DESC desc = {};
		desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		desc.MaxLOD = D3D11_FLOAT32_MAX;
		hr = device->CreateSamplerState(&desc, &sampler);
		assert(hr == S_OK);

		CD3D11_BLEND_DESC blendDesc(D3D11_DEFAULT);
		blendDesc.RenderTarget[0].BlendEnable = TRUE;
		blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		hr = device->CreateBlendState(&blendDesc, &blendState);
		assert(hr == S_OK);

		CD3D11_RASTERIZER_DESC rasterDesc(D3D11_DEFAULT);
		hr = device->CreateRasterizerState(&rasterDesc, &rasterizerState);
		assert(hr == S_OK);

		CD3D11_DEPTH_STENCIL_DESC depthStencilDesc(D3D11_DEFAULT);
		depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
		hr = device->CreateDepthStencilState(&depthStencilDesc, &depthStencilState);
		assert(hr == S_OK);
	}

	this->device = device;
	this->context = context;

	_instance = this;

	//FrameCapture::instance()->SetFramesToCapture(20);

	return true;
}

void ParticleSystem::Update(float deltaTime, float totalTime)
{
	totalEmitCount = 0;
	for (auto iPool = pools.begin(); iPool != pools.end(); ++iPool)
	{
		ParticlePool& pool = *iPool;
		for (auto iEmitter = pool.emitters.begin(); iEmitter != pool.emitters.end(); ++iEmitter)
		{
			Emitter& emitter = *iEmitter;
			emitter.counter += deltaTime * emitter.emitRate;
			emitter.emitCount = static_cast<uint32_t>(emitter.counter); // floor of uint
			emitter.counter -= emitter.emitCount;
			totalEmitCount += emitter.emitCount;
		}
	}

	if (totalEmitCount > 0)
	{
		FrameCapture::instance()->BeginCapture();

		particleEmitterCS->SetShader();
		particleEmitterCS->SetFloat("totalTime", totalTime);

		for (auto iPool = pools.begin(); iPool != pools.end(); ++iPool)
		{
			ParticlePool& pool = *iPool;
			particleEmitterCS->SetUnorderedAccessView("particles", pool.bufParticlesUAV);

			for (auto iEmitter = pool.emitters.begin(); iEmitter != pool.emitters.end(); ++iEmitter)
			{
				Emitter& emitter = *iEmitter;

				if (0 == emitter.emitCount)
					continue;

				particleEmitterCS->SetFloat4("position", emitter.position);
				particleEmitterCS->SetFloat4("velocity", emitter.velocity);
				particleEmitterCS->SetInt("emitCount", emitter.emitCount);

				if (pool.particleFirstUpdate)
				{
					particleEmitterCS->SetUnorderedAccessView("deadList", pool.bufDeadListUAV, pool.particleConstants.maxParticles);
					particleEmitterCS->SetInt("deadParticles", pool.particleConstants.maxParticles);
					particleEmitterCS->CopyAllBufferData();
					pool.particleFirstUpdate = false;
				}
				else
				{
					particleEmitterCS->CopyAllBufferData();
					particleEmitterCS->SetUnorderedAccessView("deadList", pool.bufDeadListUAV);
					context->CopyStructureCount(bufEmitter, offsetof(Emitter, deadParticles), pool.bufDeadListUAV);
				}

				particleEmitterCS->DispatchByThreads(emitter.emitCount, 1, 1);
			}
		}

		FrameCapture::instance()->EndCapture();
	}

	if (!pools.empty())
	{
		particleCS->SetShader();
		particleCS->SetFloat("deltaTime", deltaTime);

		for (auto iPool = pools.begin(); iPool != pools.end(); ++iPool)
		{
			ParticlePool& pool = *iPool;

			particleCS->SetInt("maxParticles", pool.particleConstants.maxParticles);
			particleCS->SetUnorderedAccessView("particles", pool.bufParticlesUAV);
			particleCS->SetUnorderedAccessView("deadList", pool.bufDeadListUAV);
			particleCS->SetUnorderedAccessView("drawList", pool.bufDrawListUAV, 0);

			particleCS->CopyAllBufferData();
			particleCS->DispatchByThreads(pool.particleConstants.maxParticles, 1, 1);
		}

		ID3D11UnorderedAccessView* nulls[] = { nullptr, nullptr, nullptr };
		uint32_t initVals[] = { 0xffffffffu, 0xffffffffu, 0xffffffffu };
		context->CSSetUnorderedAccessViews(0, 3, nulls, initVals);
	}
}

bool ParticleSystem::Draw(const DirectX::XMFLOAT4X4& matView, const DirectX::XMFLOAT4X4& matProj)
{
	context->OMSetBlendState(blendState, 0, 0xFFFFFFFF);
	if (totalEmitCount > 0)
		FrameCapture::instance()->BeginCapture();

	if (!pools.empty())
	{
		context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		context->IASetInputLayout(nullptr);
		context->IASetIndexBuffer(bufQuadIndices, DXGI_FORMAT_R32_UINT, 0);

		context->OMSetBlendState(blendState, nullptr, 0xffffffff);
		context->RSSetState(rasterizerState);
		context->OMSetDepthStencilState(depthStencilState, 0);

		particleVS->SetShader();
		particlePS->SetShader();

		particleVS->SetMatrix4x4("view", matView);
		particleVS->SetMatrix4x4("projection", matProj);
		particleVS->CopyAllBufferData();

		particlePS->SetSamplerState("samp", sampler);

		for (auto iPool = pools.begin(); iPool != pools.end(); ++iPool)
		{
			ParticlePool& pool = *iPool;

			particleVS->SetShaderResourceView("particles", pool.bufParticlesSRV);
			particleVS->SetShaderResourceView("drawList", pool.bufDrawListSRV);

			particlePS->SetShaderResourceView("tex", pool.texSRV);

			context->CopyStructureCount(bufIndirectDrawArgs, 4, pool.bufDrawListUAV);
			context->CopyStructureCount(bufIndirectDrawArgs, 24, pool.bufDeadListUAV);

			context->DrawIndexedInstancedIndirect(bufIndirectDrawArgs, 0);
		}

		{
			ID3D11ShaderResourceView* nulls[] = { nullptr, nullptr, nullptr };
			context->VSSetShaderResources(0, 3, nulls);
		}
	}

	if (totalEmitCount > 0)
		FrameCapture::instance()->EndCapture();

	return true;
}

void ParticleSystem::ClearAllEmitters()
{
	for (auto i = pools.begin(); i != pools.end(); ++i)
	{
		i->CleanUp();
	}

	pools.clear();
	poolMap.clear();
}

void ParticleSystem::CleanUp()
{
	ClearAllEmitters();

	delete particleVS;
	delete particlePS;
	delete particleInitCS;
	delete particleEmitterCS;
	delete particleCS;

	bufQuadIndices->Release();
	bufIndirectDrawArgs->Release();

	sampler->Release();
	blendState->Release();
	rasterizerState->Release();
	depthStencilState->Release();

	FrameCapture::CleanUp();
}

bool ParticleSystem::CreateParticleEmitter(const std::wstring& particleTexture, ParticleEmitter* emitterComponent)
{
	if (poolMap.find(particleTexture) == poolMap.end())
	{
		assert(true == CreateParticlePool(1024, particleTexture));
	}

	uint32_t poolIdx = poolMap[particleTexture];
	ParticlePool& pool = pools[poolIdx];

	uint32_t emitterIdx = pool.emitters.size();
	pool.emitters.push_back(Emitter());

	emitterComponent->poolIdx = poolIdx;
	emitterComponent->emitterIdx = emitterIdx;

	return true;
}

bool ParticleSystem::CreateParticlePool(uint32_t maxParticles, const std::wstring& texFileName)
{
	if (poolMap.find(texFileName) != poolMap.end())
		return false;

	ParticlePool pool;
	pool.particleConstants.maxParticles = maxParticles;

	HRESULT hr = S_OK;

	CD3D11_BUFFER_DESC cbDesc(sizeof(pool.particleConstants), D3D11_BIND_CONSTANT_BUFFER);
	hr = device->CreateBuffer(&cbDesc, nullptr, &(pool.bufParticleConstants));
	assert(hr == S_OK);

	CD3D11_BUFFER_DESC bufDesc(
		pool.particleConstants.maxParticles * sizeof(Particle),
		D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE,
		D3D11_USAGE_DEFAULT,
		0,
		D3D11_RESOURCE_MISC_BUFFER_STRUCTURED,
		sizeof(Particle)
	);

	hr = device->CreateBuffer(&bufDesc, nullptr, &pool.bufParticles);
	assert(hr == S_OK);

	bufDesc.BindFlags = D3D11_BIND_UNORDERED_ACCESS;
	bufDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	bufDesc.ByteWidth = pool.particleConstants.maxParticles * sizeof(uint32_t);
	bufDesc.StructureByteStride = sizeof(uint32_t);

	hr = device->CreateBuffer(&bufDesc, nullptr, &pool.bufDeadList);
	assert(hr == S_OK);

	bufDesc.BindFlags |= D3D11_BIND_SHADER_RESOURCE;

	hr = device->CreateBuffer(&bufDesc, nullptr, &pool.bufDrawList);
	assert(hr == S_OK);

	CD3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc(
		(ID3D11Buffer*)nullptr,
		DXGI_FORMAT_UNKNOWN,
		0, pool.particleConstants.maxParticles,
		D3D11_BUFFER_UAV_FLAG_APPEND
	);

	hr = device->CreateUnorderedAccessView(pool.bufParticles, nullptr, &pool.bufParticlesUAV);
	assert(hr == S_OK);

	hr = device->CreateShaderResourceView(pool.bufParticles, nullptr, &pool.bufParticlesSRV);
	assert(hr == S_OK);

	hr = device->CreateUnorderedAccessView(pool.bufDeadList, nullptr, &pool.bufDeadListUAV);
	assert(hr == S_OK);

	hr = device->CreateUnorderedAccessView(pool.bufDrawList, &uavDesc, &pool.bufDrawListUAV);
	assert(hr == S_OK);

	hr = device->CreateShaderResourceView(pool.bufDrawList, nullptr, &pool.bufDrawListSRV);
	assert(hr == S_OK);

	particleInitCS->SetUnorderedAccessView("particles", pool.bufParticlesUAV);
	particleInitCS->SetUnorderedAccessView("deadList", pool.bufDeadListUAV);
	particleInitCS->SetShader();
	context->Dispatch((pool.particleConstants.maxParticles + 1023) / 1024, 1, 1);
	{
		ID3D11UnorderedAccessView* nullUAV = nullptr;
		uint32_t val = D3D11_KEEP_UNORDERED_ACCESS_VIEWS;
		context->CSSetUnorderedAccessViews(0, 1, &nullUAV, &val);
	}
	context->CSSetShader(nullptr, nullptr, 0);

	pool.bufDeadListUAV->Release();

	hr = device->CreateUnorderedAccessView(pool.bufDeadList, &uavDesc, &pool.bufDeadListUAV);
	assert(hr == S_OK);

	hr = DirectX::CreateWICTextureFromFile(device, texFileName.c_str(), nullptr, &pool.texSRV);
	assert(hr == S_OK);

	pool.particleFirstUpdate = true;

	uint32_t idx = pools.size();
	pools.push_back(pool);
	poolMap.insert(std::pair<std::wstring, uint32_t>{texFileName, idx});

	return true;
}
