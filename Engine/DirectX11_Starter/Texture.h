#pragma once

#include <cstdint>

struct ID3D11Device;
struct ID3D11ShaderResourceView;

class RenderingSystem;
class Material;

enum TextureType
{
	TextureUnknown,
	Texture1D,
	Texture2D,
	Texture3D
};

struct TextureInfo
{
	TextureType		type;
	uint32_t		width;
	uint32_t		height;
	uint32_t		depth;
	uint32_t		arraySize;
	uint32_t		mipLevels;
};

class Texture
{
private:
	friend class RenderingSystem;
	friend class Material;

	Texture() : valid(false), texSRV(nullptr) {}
	~Texture();

	void CleanUp();

	bool LoadTextureFromFile(const wchar_t* filename, ID3D11Device* device);
	bool LoadTextureFromMemory(const void* data, int width, int height, ID3D11Device* device);

public:

	bool IsValid() const { return valid; }

	const TextureInfo* GetInfo() const { return &texInfo; }

private:
	bool						valid;
	ID3D11ShaderResourceView*	texSRV;

	TextureInfo					texInfo;
};
