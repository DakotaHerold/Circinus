#include "Texture.h"

#include <d3d11.h>
#include <WICTextureLoader.h>
#include <DDSTextureLoader.h>
#include <cwctype>

#define HR(x) if ((x) != S_OK) return false;

using namespace DirectX;

Texture::~Texture()
{
	CleanUp();
}

void Texture::CleanUp()
{
	texInfo = {};
	valid = false;
	if (nullptr == texSRV) return;
	texSRV->Release();
	texSRV = nullptr;
}

// TODO  informations like width and height
bool Texture::LoadTextureFromFile(const wchar_t * filename, ID3D11Device * device)
{
	CleanUp();

	size_t len = wcslen(filename);
	if (len > 4 &&
		filename[len - 4] == L'.' &&
		std::towlower(filename[len - 3]) == L'd' &&
		std::towlower(filename[len - 2]) == L'd' &&
		std::towlower(filename[len - 1]) == L's')
	{
		HR(CreateDDSTextureFromFile(device, filename, nullptr, &texSRV));
	}
	else
	{
		HR(CreateWICTextureFromFile(device, filename, nullptr, &texSRV));
	}

	{
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};

		ID3D11Resource* res = nullptr;
		texSRV->GetResource(&res);

		if (nullptr == res) return false;

		texSRV->GetDesc(&srvDesc);

		switch (srvDesc.ViewDimension)
		{
		case D3D11_SRV_DIMENSION_TEXTURE1D:
		case D3D11_SRV_DIMENSION_TEXTURE1DARRAY:
		{
			D3D11_TEXTURE1D_DESC texDesc = {};
			ID3D11Texture1D* tex = nullptr;
			HR(res->QueryInterface(&tex));
			tex->GetDesc(&texDesc);
			tex->Release();
			texInfo = TextureInfo{
				Texture1D,
				texDesc.Width,
				1,
				1,
				texDesc.ArraySize,
				texDesc.MipLevels
			};
		}
		break;
		case D3D11_SRV_DIMENSION_TEXTURE2D:
		case D3D11_SRV_DIMENSION_TEXTURE2DARRAY:
		case D3D11_SRV_DIMENSION_TEXTURE2DMS:
		case D3D11_SRV_DIMENSION_TEXTURE2DMSARRAY:
		case D3D11_SRV_DIMENSION_TEXTURECUBE:
		case D3D11_SRV_DIMENSION_TEXTURECUBEARRAY:
		{
			D3D11_TEXTURE2D_DESC texDesc = {};
			ID3D11Texture2D* tex = nullptr;
			HR(res->QueryInterface(&tex));
			tex->GetDesc(&texDesc);
			tex->Release();
			texInfo = TextureInfo{
				Texture2D,
				texDesc.Width,
				texDesc.Height,
				1,
				texDesc.ArraySize,
				texDesc.MipLevels
			};
		}
		break;
		case D3D11_SRV_DIMENSION_TEXTURE3D:
		{
			D3D11_TEXTURE3D_DESC texDesc = {};
			ID3D11Texture3D* tex = nullptr;
			HR(res->QueryInterface(&tex));
			tex->GetDesc(&texDesc);
			tex->Release();
			texInfo = TextureInfo{
				Texture3D,
				texDesc.Width,
				texDesc.Height,
				texDesc.Depth,
				1,
				texDesc.MipLevels
			};
		}
		break;
		default:
			res->Release();
			return false;
		}

		res->Release();
	}

	valid = true;
	return true;
}

bool Texture::LoadTextureFromMemory(const void * data, int width, int height, ID3D11Device * device)
{
	CleanUp();
	// TODO
	return false;
}

