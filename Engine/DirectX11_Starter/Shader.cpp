#include "Shader.h"

#include <d3d11.h>
#include <d3dcompiler.h>
#include <d3dx11effect.h>
#include <vector>

#define HR(x) if ((x) != S_OK) return false;

Shader::~Shader()
{
	CleanUp();
}

void Shader::CleanUp()
{
	valid = false;
	if (nullptr != effect)
	{
		effect->Release();
		effect = nullptr;
	}
	if (nullptr != layout)
	{
		layout->Release();
		layout = nullptr;
	}
}

bool Shader::LoadShaderFromCSO(ID3D11Device * device, const wchar_t * filename)
{
	ID3DBlob* blob = nullptr;

	HR(D3DReadFileToBlob(filename, &blob));

	HR(D3DX11CreateEffectFromMemory(blob->GetBufferPointer(), blob->GetBufferSize(), 0, device, &effect));

	D3DX11_PASS_SHADER_DESC desc = {};
	effect->GetTechniqueByIndex(0)->GetPassByIndex(0)->GetVertexShaderDesc(&desc);
	auto vs = &(desc.pShaderVariable[desc.ShaderIndex]);
	
	D3DX11_EFFECT_SHADER_DESC shdrDesc = {};
	HR(vs->GetShaderDesc(0, &shdrDesc));

	uint32_t numEntry = shdrDesc.NumInputSignatureEntries;

	std::vector<D3D11_INPUT_ELEMENT_DESC> inputLayoutDesc;
	for (unsigned int i = 0; i< numEntry; i++)
	{
		D3D11_SIGNATURE_PARAMETER_DESC paramDesc;
		HR(vs->GetInputSignatureElementDesc(0, i, &paramDesc));

		// Fill out input element desc
		D3D11_INPUT_ELEMENT_DESC elementDesc;
		elementDesc.SemanticName = paramDesc.SemanticName;
		elementDesc.SemanticIndex = paramDesc.SemanticIndex;
		elementDesc.InputSlot = 0;
		elementDesc.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
		elementDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		elementDesc.InstanceDataStepRate = 0;

		// Determine DXGI format
		if (paramDesc.Mask == 1)
		{
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32_FLOAT;
		}
		else if (paramDesc.Mask <= 3)
		{
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32G32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32G32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32G32_FLOAT;
		}
		else if (paramDesc.Mask <= 7)
		{
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
		}
		else if (paramDesc.Mask <= 15)
		{
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		}

		// Save element desc
		inputLayoutDesc.push_back(elementDesc);
	}

	HR(device->CreateInputLayout(&inputLayoutDesc[0], numEntry, shdrDesc.pBytecode, shdrDesc.BytecodeLength, &layout));

	valid = true;

	return true;
}
