#include "Mesh.h"
#include "AssimpLoader.h"

#include <d3d11.h>

#define HR(x) if ((x) != S_OK) return false;

// For the DirectX Math library
using namespace DirectX;

Mesh::~Mesh()
{
	CleanUp();
}

void Mesh::CleanUp()
{
	valid = false;
	if (nullptr != vertexBuffer) 
	{ 
		vertexBuffer->Release(); 
		vertexBuffer = nullptr;
	}
	if (nullptr != indexBuffer)
	{
		indexBuffer->Release();
		indexBuffer = nullptr;
	}
	indexCount = 0;
}

bool Mesh::LoadFromMemory(Vertex vertices[], int numVerts, unsigned int indices[], int numIndices, ID3D11Device * device)
{
	CleanUp();
	// Create the VERTEX BUFFER description -----------------------------------
	// - The description is created on the stack because we only need
	//    it to create the buffer.  The description is then useless.
	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(Vertex) * numVerts;       // 3 = number of vertices in the buffer
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER; // Tells DirectX this is a vertex buffer
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;

	// Create the proper struct to hold the initial vertex data
	// - This is how we put the initial data into the buffer
	D3D11_SUBRESOURCE_DATA initialVertexData;
	initialVertexData.pSysMem = vertices;

	// Actually create the buffer with the initial data
	// - Once we do this, we'll NEVER CHANGE THE BUFFER AGAIN
	HR(device->CreateBuffer(&vbd, &initialVertexData, &vertexBuffer));

	// Create the INDEX BUFFER description ------------------------------------
	// - The description is created on the stack because we only need
	//    it to create the buffer.  The description is then useless.
	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(int) * numIndices;         // 3 = number of indices in the buffer
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER; // Tells DirectX this is an index buffer
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;

	// Create the proper struct to hold the initial index data
	// - This is how we put the initial data into the buffer
	D3D11_SUBRESOURCE_DATA initialIndexData;
	initialIndexData.pSysMem = indices;
	indexCount = numIndices;

	// Actually create the buffer with the initial data
	// - Once we do this, we'll NEVER CHANGE THE BUFFER AGAIN
	HR(device->CreateBuffer(&ibd, &initialIndexData, &indexBuffer));

	valid = true;
	return true;
}

bool Mesh::LoadFromFile(const char * filename, ID3D11Device * device)
{
	CleanUp();
	AssimpLoader loader;
	loader.LoadFromFile(filename);

	unsigned long dataSize = loader.GetVerticesCount() * sizeof(Vertex);
	void* pData = malloc(dataSize);

	loader.FillInVerticesData(pData);

	if (!LoadFromMemory(reinterpret_cast<Vertex*>(pData), loader.GetVerticesCount(), const_cast<unsigned int*>(loader.GetIndices()), loader.GetIndicesCount(), device))
	{
		free(pData);
		return false;
	}

	free(pData);
	return true;
}
