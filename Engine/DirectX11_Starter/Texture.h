#pragma once

struct ID3D11Device;
struct ID3D11ShaderResourceView;

class Texture
{
public:
	Texture() : texSRV(nullptr) {}

	Texture(const wchar_t* filename, ID3D11Device* device);

	~Texture();

	void LoadTextureFromFile(const wchar_t* filename, ID3D11Device* device);

	void LoadTextureFromMemory(const void* data, int width, int height, ID3D11Device* device);

	void Release();


	ID3D11ShaderResourceView*	GetSRV() const { return texSRV; }

private:
	ID3D11ShaderResourceView*	texSRV;
};
