#pragma once

#include <DirectXMath.h>
#include "Vertex.h"
#include "DirectXGameCore.h"
#include <d3d11.h>
#include <iostream>
#include <fstream>
#include <vector>

#if defined(DEBUG) || defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif

using namespace std; 

class Mesh 
{
public:
	Mesh() : vertexBuffer(nullptr), indexBuffer(nullptr), indexCount(0) {}
	~Mesh();
	Mesh(Vertex vertices[], int numVerts, unsigned int indices[], int numIndices, ID3D11Device * device);
	Mesh(char* filename, ID3D11Device* device);
	ID3D11Buffer* GetVertexBuffer();
	ID3D11Buffer* GetIndexBuffer();
	int GetIndexCount();

	void LoadFromMemory(Vertex vertices[], int numVerts, unsigned int indices[], int numIndices, ID3D11Device * device);
	void LoadFromFile(char* filename, ID3D11Device* device);

private:
	ID3D11Buffer* vertexBuffer; 
	ID3D11Buffer* indexBuffer;
	int indexCount; 
};

