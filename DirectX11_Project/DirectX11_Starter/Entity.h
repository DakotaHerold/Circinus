#pragma once
#include "Mesh.h"
#include "Material.h"


using namespace DirectX;

class Entity
{
public:
	Entity();
	~Entity();
	Entity(Mesh* inputMesh, Material* inputMaterial);
	//attributes 
	Mesh* mesh;
	Material* material;

	//getters and setters
	XMFLOAT4X4 getPosition();
	XMFLOAT4X4 getRotation();
	XMFLOAT4X4 getScale();
	XMFLOAT4X4 getWorldMatrix();

	void setPosition(float x, float y, float z);
	void setRotation(XMFLOAT4X4 newRot);
	void setScale(XMFLOAT3 newScale);
	void setWorldMatrix(XMFLOAT4X4 newWorldMatrix);

	//Class Specific functions 
	void updateScene();
	void drawScene(ID3D11DeviceContext* deviceContext);
	void move(XMFLOAT4 force);
	void scale(XMFLOAT4 nscalar);
	void rotate(XMFLOAT4 axis, float angle);
	void prepareMaterial(XMFLOAT4X4 view, XMFLOAT4X4 proj);

private:
	XMFLOAT4X4 position;
	XMFLOAT4X4 rotation;
	XMFLOAT4X4 scalar;
	XMFLOAT4X4 worldMatrix;



};

