#include "Material.h"



Material::Material()
{
	vertexShader = nullptr; 
	pixelShader = nullptr; 
	SRV = nullptr; 
	samplerState = nullptr; 
	file = nullptr; 
}

Material::Material(SimpleVertexShader * vShader, SimplePixelShader* pShader)
{
	vertexShader = vShader; 
	pixelShader = pShader; 
}

Material::Material(SimpleVertexShader* vShader, SimplePixelShader* pShader, ID3D11Device* device, ID3D11DeviceContext* deviceContext, const wchar_t* filePath, bool ddsFile)
{
	vertexShader = vShader;
	pixelShader = pShader;
	file = filePath; 


	//check for .dds file
	if (ddsFile){
		CreateDDSTextureFromFile(device, deviceContext, filePath, 0, &SRV);
	}
	else {
		CreateWICTextureFromFile(device, deviceContext, filePath, 0, &SRV); 
	}
	
	
	D3D11_SAMPLER_DESC sampleDescription = {}; 
//	ZeroMemory(&sampleDescription, sizeof(sampleDescription)); 

	sampleDescription.AddressU = D3D11_TEXTURE_ADDRESS_WRAP; 
	sampleDescription.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampleDescription.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampleDescription.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampleDescription.MaxLOD = D3D11_FLOAT32_MAX; 

	device->CreateSamplerState(&sampleDescription, &samplerState);
}
//dds file not specified, assume false
Material::Material(SimpleVertexShader* vShader, SimplePixelShader* pShader, ID3D11Device* device, ID3D11DeviceContext* deviceContext, const wchar_t* filePath) :
	Material(vShader, pShader, device, deviceContext, filePath, false){

}

Material::~Material()
{
	SRV->Release(); 
}

ID3D11ShaderResourceView * Material::getTextureSRV()
{
	return SRV;
}

ID3D11SamplerState * Material::getSamplerState()
{
	return samplerState;
}

const wchar_t * Material::getFileName()
{
	return file;
}
