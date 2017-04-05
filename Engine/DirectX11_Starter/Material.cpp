#include "Material.h"

#include <d3d11.h>
#include <d3dx11effect.h>
#include "Shader.h"
#include "Texture.h"

#define HR(x) if ((x) != S_OK) return false;

Material::~Material()
{
	CleanUp();
}

void Material::CleanUp()
{
	shader = nullptr;

	if (nullptr != effect)
	{
		effect->Release();
		effect = nullptr;
	}

	cbs.clear();
	texs.clear();
	cbTable.clear();
	varTable.clear();
	texTable.clear();
}

bool Material::InitWithShader(ID3D11Device* device, Shader * shader, const std::unordered_map<std::string, ConstantBuffer*>* preBoundCBs)
{
	CleanUp();

	if (!shader->IsValid())
		return false;

	this->shader = shader;
	HR(shader->effect->CloneEffect(0, &effect));

	D3DX11_EFFECT_DESC desc = {};
	HR(effect->GetDesc(&desc));

	cbCount = desc.ConstantBuffers;

	uint32_t varCount = desc.GlobalVariables;

	for (uint32_t i = 0; i < cbCount; i++)
	{
		auto cb = effect->GetConstantBufferByIndex(i);
		D3DX11_EFFECT_VARIABLE_DESC desc = {};
		cb->GetDesc(&desc);

		if (nullptr != preBoundCBs)
		{
			auto iter = preBoundCBs->find(desc.Name);
			if (iter != preBoundCBs->end())
			{
				cb->SetConstantBuffer(iter->second->GetBuffer());
				continue;
			}
		}

		auto type = cb->GetType();
		D3DX11_EFFECT_TYPE_DESC typeDesc = {};
		HR(type->GetDesc(&typeDesc));

		cbs.push_back(ConstantBuffer());
		ConstantBuffer& newCB = cbs.back();

		if (!newCB.Init(device, typeDesc.UnpackedSize))
			return false;


		HR(cb->SetConstantBuffer(newCB.GetBuffer()));

		for (uint32_t m = 0; m < typeDesc.Members; m++)
		{
			auto mtype = type->GetMemberTypeByIndex(m);
			D3DX11_EFFECT_TYPE_DESC mtypeDesc = {};
			HR(mtype->GetDesc(&mtypeDesc));

			auto var = effect->GetVariableByName(type->GetMemberName(m));
			D3DX11_EFFECT_VARIABLE_DESC varDesc = {};
			HR(var->GetDesc(&varDesc));

			varTable.insert(std::pair<std::string, Variable>(
				varDesc.Name,
				Variable{ i, varDesc.BufferOffset, mtypeDesc.UnpackedSize }
			));
		}
	}

	for (uint32_t i = 0; i < varCount; i++)
	{
		auto var = effect->GetVariableByIndex(i);

		D3DX11_EFFECT_VARIABLE_DESC varDesc = {};
		HR(var->GetDesc(&varDesc));

		auto type = var->GetType();
		if (nullptr == type) return false;

		D3DX11_EFFECT_TYPE_DESC typeDesc = {};
		HR(type->GetDesc(&typeDesc));

		switch (typeDesc.Type)
		{
		case D3D_SVT_TEXTURE:
		case D3D_SVT_TEXTURE1D:
		case D3D_SVT_TEXTURE1DARRAY:
		case D3D_SVT_TEXTURE2D:
		case D3D_SVT_TEXTURE2DARRAY:
		case D3D_SVT_TEXTURE2DMS:
		case D3D_SVT_TEXTURE2DMSARRAY:
		case D3D_SVT_TEXTURE3D:
		case D3D_SVT_TEXTURECUBE:
		case D3D_SVT_TEXTURECUBEARRAY:
		{
			auto srv = var->AsShaderResource();
			if (nullptr == srv)
				return false;

			texs.push_back(srv);
			texTable.insert(std::pair<std::string, uint32_t>(
				varDesc.Name,
				static_cast<uint32_t>(texs.size() - 1)
				));
		}
		break;
		default:
			break;
		}
	}

	valid = true;

	return true;
}

bool Material::UpdateConstants(ID3D11DeviceContext * context)
{
	for (auto i = cbs.begin(); i != cbs.end(); ++i)
	{
		i->UploadBuffer(context);
	}

	return true;
}

bool Material::Apply(ID3D11DeviceContext * context)
{
	if (!UpdateConstants(context))
		return false;

	effect->GetTechniqueByIndex(0)->GetPassByIndex(0)->Apply(0, context);
	return true;
}

bool Material::SetData(const std::string & name, const void * data, unsigned int size)
{
	if (nullptr == data) return false;
	auto iter = varTable.find(name);
	if (varTable.end() == iter) return false;

	Variable& var = iter->second;

	if (!cbs[var.cbIndex].UpdateData(data, var.offset, size))
		return false;

	return true;
}

bool Material::SetInt(const std::string & name, int data)
{
	return SetData(name, &data, sizeof(int));
}

bool Material::SetFloat(const std::string & name, float data)
{
	return SetData(name, &data, sizeof(float));
}

bool Material::SetFloat2(const std::string & name, const float data[2])
{
	return SetData(name, data, sizeof(float) * 2);
}

bool Material::SetFloat2(const std::string & name, const DirectX::XMFLOAT2& data)
{
	return SetData(name, &data, sizeof(DirectX::XMFLOAT2));
}

bool Material::SetFloat3(const std::string & name, const float data[3])
{
	return SetData(name, data, sizeof(float) * 3);
}

bool Material::SetFloat3(const std::string & name, const DirectX::XMFLOAT3& data)
{
	return SetData(name, &data, sizeof(DirectX::XMFLOAT3));
}

bool Material::SetFloat4(const std::string & name, const float data[4])
{
	return SetData(name, data, sizeof(float) * 4);
}

bool Material::SetFloat4(const std::string & name, const DirectX::XMFLOAT4& data)
{
	return SetData(name, &data, sizeof(DirectX::XMFLOAT4));
}

bool Material::SetMatrix4x4(const std::string & name, const float data[16])
{
	return SetData(name, data, sizeof(float) * 16);
}

bool Material::SetMatrix4x4(const std::string & name, const DirectX::XMFLOAT4X4& data)
{
	return SetData(name, &data, sizeof(DirectX::XMFLOAT4X4));
}

bool Material::SetTexture(const std::string & name, Texture * tex)
{
	if (nullptr == tex || !tex->IsValid()) return false;
	auto iter = texTable.find(name);
	if (texTable.end() == iter) return false;

	auto srv = texs[iter->second];
	srv->SetResource(tex->texSRV);

	return true;
}

