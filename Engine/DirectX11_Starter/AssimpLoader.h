#pragma once
#include "Vertex.h"

class AssimpLoader
{
public:
	AssimpLoader() : vertices(nullptr), normals(nullptr), tangents(nullptr), uvs(nullptr), indices(nullptr) {}

	~AssimpLoader();

	void Release();

	void LoadFromFile(const char* filename);

	void FillInVerticesData(void* pDest) const;
	size_t GetVerticesCount() const { return numVertices; }

	const unsigned int* GetIndices() const { return indices; }
	size_t GetIndicesCount() const { return numIndices; }

private:
	typedef DirectX::XMFLOAT3 Vector3D;
	typedef DirectX::XMFLOAT2 Vector2D;

	Vector3D*		vertices;
	Vector3D*		normals;
	Vector3D*		tangents;
	Vector2D*		uvs;
	unsigned int*	indices;
	size_t			numVertices;
	size_t			numIndices;
};
