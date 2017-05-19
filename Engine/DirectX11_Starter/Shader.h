#pragma once
struct ID3D11Device;
struct ID3D11InputLayout;
struct ID3DX11Effect;


class RenderingSystem;
class Material;

// shader wrap that use d3dx 11 effect
class Shader
{
private:
	friend class RenderingSystem;
	friend class Material;

	Shader() : valid(false), effect(nullptr), layout(nullptr) {}
	~Shader();

	void CleanUp();

	bool LoadShaderFromCSO(ID3D11Device* device, const wchar_t* filename);

public:

	bool IsValid() const { return valid; }


private:
	bool					valid;

	ID3DX11Effect*			effect;
	ID3D11InputLayout*		layout;

};