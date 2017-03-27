#include "Texture.h"
#include "DirectXGameCore.h"

#include <d3d11.h>
#include <WICTextureLoader.h>
#include <DDSTextureLoader.h>

using namespace DirectX;

Texture::Texture(const wchar_t * filename, ID3D11Device * device)
	: Texture()
{
	LoadTextureFromFile(filename, device);
}

Texture::~Texture()
{
	Release();
}

// TODO  informations like width and height
// TODO  dds format support
void Texture::LoadTextureFromFile(const wchar_t * filename, ID3D11Device * device)
{
	HR(CreateWICTextureFromFile(device, filename, nullptr, &texSRV));
}

void Texture::LoadTextureFromMemory(const void * data, int width, int height, ID3D11Device * device)
{
	// TODO
}

void Texture::Release()
{
	if (nullptr == texSRV) return;
	texSRV->Release();
	texSRV = nullptr;
}
