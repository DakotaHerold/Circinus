#pragma once

#include <DirectXMath.h>
#include <DirectXCollision.h>
#include "Vertex.h"
#include <string>

struct ID3D11Device;
struct ID3D11Buffer;

class RenderingSystem;
#ifdef HAS_GUI
class GUI;
#endif


class Mesh
{
private:
	friend class RenderingSystem;
#ifdef HAS_GUI
	friend class GUI;
#endif

	Mesh() : valid(false), vertexBuffer(nullptr), indexBuffer(nullptr), indexCount(0), bounds() {}
	~Mesh();

	void CleanUp();

	bool LoadFromMemory(Vertex vertices[], int numVerts, unsigned int indices[], int numIndices, ID3D11Device * device);
	bool LoadFromFile(const wchar_t* filename, ID3D11Device* device);

public:

	bool IsValid() const { return valid; }

	ID3D11Buffer* GetVertexBuffer() { return vertexBuffer; }
	ID3D11Buffer* GetIndexBuffer() { return indexBuffer; }
	int GetIndexCount() { return indexCount; }

	const DirectX::BoundingBox& GetBounds() const { return bounds; }



private:
	bool			valid;
	ID3D11Buffer*	vertexBuffer;
	ID3D11Buffer*	indexBuffer;
	int				indexCount;
	DirectX::BoundingBox bounds;

	std::string		filename;
	std::wstring	filename_w;
};

