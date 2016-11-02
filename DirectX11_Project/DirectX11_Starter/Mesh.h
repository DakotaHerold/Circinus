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
	Mesh();
	~Mesh();
	Mesh(Vertex vertices[], int numVerts, unsigned int tempIndices[], int numIndices, ID3D11Device* device);
	Mesh(char* filename, ID3D11Device* device);
	ID3D11Buffer* GetVertexBuffer();
	ID3D11Buffer* GetIndexBuffer();
	int GetIndexCount();

private:
	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;
	int indexCount;


};

