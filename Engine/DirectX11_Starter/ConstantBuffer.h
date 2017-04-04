#pragma once

#include <cstdint>

struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11Buffer;

struct ConstantBuffer
{
	ID3D11Buffer*	buffer;
	uint8_t*		cache;
	uint32_t		size;
	bool			dirty;

	ConstantBuffer() : buffer(nullptr), cache(nullptr), size(0), dirty(false) {}

	bool Init(ID3D11Device* device, uint32_t size);

	void CleanUp();

	bool UpdateData(const void* data, uint32_t offset, uint32_t size);

	void UploadBuffer(ID3D11DeviceContext* context);

	~ConstantBuffer();
};