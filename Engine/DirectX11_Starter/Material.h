#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <DirectXMath.h>

struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11Buffer;
struct ID3D11ShaderResourceView;
struct ID3DX11Effect;
struct ID3DX11EffectShaderResourceVariable;

class RenderingSystem;
class Texture;
class Shader;

class Material
{
private:
	friend class RenderingSystem;

	Material() : valid(false), shader(nullptr), effect(nullptr) {}
	~Material();

	void CleanUp();

	bool InitWithShader(ID3D11Device* device, Shader* shader);

	bool UpdateConstants(ID3D11DeviceContext* context);

	bool Apply(ID3D11DeviceContext* context);

public:

	bool IsValid() const { return valid; }

	bool SetData(const std::string& name, const void* data, unsigned int size);

	bool SetInt(const std::string& name, int data);
	bool SetFloat(const std::string& name, float data);
	bool SetFloat2(const std::string& name, const float data[2]);
	bool SetFloat2(const std::string& name, const DirectX::XMFLOAT2& data);
	bool SetFloat3(const std::string& name, const float data[3]);
	bool SetFloat3(const std::string& name, const DirectX::XMFLOAT3& data);
	bool SetFloat4(const std::string& name, const float data[4]);
	bool SetFloat4(const std::string& name, const DirectX::XMFLOAT4& data);
	bool SetMatrix4x4(const std::string& name, const float data[16]);
	bool SetMatrix4x4(const std::string& name, const DirectX::XMFLOAT4X4& data);
	bool SetTexture(const std::string& name, Texture* tex);

private:
	bool					valid;
	Shader*					shader;
	ID3DX11Effect*			effect;

	uint32_t				cbCount;

	struct ConstantBuffer
	{
		ID3D11Buffer*	buffer;
		uint8_t*		cache;
		uint32_t		size;
		bool			dirty;

		ConstantBuffer() : buffer(nullptr), cache(nullptr), size(0), dirty(false) {}

		~ConstantBuffer();
	};

	struct Variable
	{
		uint32_t	cbIndex;
		uint32_t	offset;
		uint32_t	size;
	};


	std::vector<ConstantBuffer>			cbs;
	std::vector<void*>					cbCaches;
	std::vector<ID3DX11EffectShaderResourceVariable*>	texs;

	std::unordered_map<std::string, uint32_t> cbTable;
	std::unordered_map<std::string, Variable> varTable;

	std::unordered_map<std::string, uint32_t> texTable;

};

