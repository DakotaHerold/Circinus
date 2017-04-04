#include "ConstantBuffer.h"

#include <d3d11.h>

#define HR(x) if ((x) != S_OK) return false;

bool ConstantBuffer::Init(ID3D11Device * device, uint32_t size)
{
	CleanUp();

	D3D11_BUFFER_DESC bufDesc = {};
	bufDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufDesc.ByteWidth = size;
	bufDesc.Usage = D3D11_USAGE_DEFAULT;

	HR(device->CreateBuffer(&bufDesc, 0, &buffer));
	cache = new uint8_t[size];
	this->size = size;
	dirty = true;

	return true;
}

void ConstantBuffer::CleanUp()
{
	delete[] cache;
	cache = nullptr;

	if (nullptr != buffer)
	{
		buffer->Release();
		buffer = nullptr;
	}

	size = 0;
	dirty = false;
}

bool ConstantBuffer::UpdateData(const void* data, uint32_t offset, uint32_t size)
{
	if (size > this->size) return false;
	memcpy(cache + offset, data, size);
	dirty = true;
	return true;
}

void ConstantBuffer::UploadBuffer(ID3D11DeviceContext * context)
{
	if (dirty)
	{
		context->UpdateSubresource(buffer, 0, nullptr, cache, 0, 0);
		dirty = false;
	}
}

ConstantBuffer::~ConstantBuffer()
{
	CleanUp();
}
