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

	static const DirectX::XMFLOAT3 white;
	static const DirectX::XMFLOAT3 red;

public:
	void Reset();
	void Render(const DirectX::XMFLOAT4X4& matView, const DirectX::XMFLOAT4X4& matProj);

	void Draw(const DirectX::BoundingBox& bound, const DirectX::XMFLOAT3& color = white);
	void Draw(const DirectX::BoundingFrustum& bound, const DirectX::XMFLOAT3& color = white);
	void Draw(const DirectX::BoundingOrientedBox& bound, const DirectX::XMFLOAT3& color = white);
	void Draw(const DirectX::BoundingSphere& bound, const DirectX::XMFLOAT3& color = white);
	
private:
	inline uint32_t AddVertex(const DirectX::XMFLOAT3& vert, const DirectX::XMFLOAT3& color) { pVertices[curVertex] = Vertex{ vert, color }; return (curVertex++); }
	inline uint32_t AddIndex(uint32_t index) { pIndices[curIndex] = index; return (curIndex++); }

	void AddCube(DirectX::CXMMATRIX& matWorld, const DirectX::XMFLOAT3& color);

	void AddRing(
		DirectX::FXMVECTOR origin,
		DirectX::FXMVECTOR majorAxis,
		DirectX::FXMVECTOR minorAxis,
		const DirectX::XMFLOAT3& color);

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

	struct Vertex
	{
		DirectX::XMFLOAT3	position;
		DirectX::XMFLOAT3	color;
	};

	Vertex*					pVertices;
	uint16_t*				pIndices;
};