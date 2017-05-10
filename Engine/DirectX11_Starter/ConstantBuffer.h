#pragma once

#include <cstdint>

struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11Buffer;

struct ConstantBuffer
{
public:

	ConstantBuffer()
		:
		buffer(nullptr),
		cache(nullptr),
		size(0),
		dirty(false),
		externalCache(false)
	{}

	~ConstantBuffer();

	bool Init(ID3D11Device* device, uint32_t size, void* cacheBuf = nullptr);

	void CleanUp();

	void MarkDirty() { dirty = true; }

	bool UpdateData(const void* data, uint32_t offset, uint32_t size);

	void UploadBuffer(ID3D11DeviceContext* context);

	ID3D11Buffer* GetBuffer() const { return buffer; }

private:
	ID3D11Buffer*	buffer;
	uint8_t*		cache;
	uint32_t		size;
	bool			dirty;
	bool			externalCache;
};