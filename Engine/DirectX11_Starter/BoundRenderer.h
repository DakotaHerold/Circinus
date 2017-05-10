#pragma once

#include <DirectXCollision.h>

struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11Buffer;
struct ID3DX11Effect;
struct ID3D11InputLayout;

class BoundRenderer
{
private:
	static BoundRenderer* _instance;

public:
	static BoundRenderer* instance() { return _instance; }

public:
	void Reset();
	void Render(const DirectX::XMFLOAT4X4& matView, const DirectX::XMFLOAT4X4& matProj);

	void Draw(const DirectX::BoundingBox& bound);
	void Draw(const DirectX::BoundingFrustum& bound);
	void Draw(const DirectX::BoundingOrientedBox& bound);
	void Draw(const DirectX::BoundingSphere& bound);
	
private:
	inline uint32_t AddVertex(const DirectX::XMFLOAT3& vert) { pVertices[curVertex] = vert; return (curVertex++); }
	inline uint32_t AddIndex(uint32_t index) { pIndices[curIndex] = index; return (curIndex++); }

	void AddCube(DirectX::CXMMATRIX& matWorld);

	/*void AddRing(
		const DirectX::XMFLOAT3& origin,
		const DirectX::XMFLOAT3& majorAxis,
		const DirectX::XMFLOAT3& minorAxis);*/

	void AddRing(
		DirectX::FXMVECTOR origin,
		DirectX::FXMVECTOR majorAxis,
		DirectX::FXMVECTOR minorAxis);

private:

	friend class RenderingSystem;

	void Init(ID3D11Device* device, ID3D11DeviceContext* context);
	void CleanUp();

private:
	ID3D11Device*			device;
	ID3D11DeviceContext*	context;

	ID3DX11Effect*			effect;
	ID3D11InputLayout*		inputLayout;

	ID3D11Buffer*			vertexBuffer;
	ID3D11Buffer*			indexBuffer;
	ID3D11Buffer*			constantBuffer;

	uint32_t				maxVertices;
	uint32_t				maxIndices;

	uint32_t				curVertex;
	uint32_t				curIndex;

	DirectX::XMFLOAT3*		pVertices;
	uint16_t*				pIndices;
};