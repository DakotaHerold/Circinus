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

	// if you want to use your own memory as cache, pass the pointer to cacheBuf
	bool Init(ID3D11Device* device, uint32_t size, void* cacheBuf = nullptr);

	void CleanUp();

	void MarkDirty() { dirty = true; }

	// update in-memory cache
	bool UpdateData(const void* data, uint32_t offset, uint32_t size);

	// upload it to GPU
	void UploadBuffer(ID3D11DeviceContext* context);

	ID3D11Buffer* GetBuffer() const { return buffer; }

private:
	ID3D11Buffer*	buffer;
	uint8_t*		cache;
	uint32_t		size;
	bool			dirty;
	bool			externalCache;
};