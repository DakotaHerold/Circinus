#pragma once

#include <DirectXMath.h>
#include <DirectXCollision.h>
#include "Component.h"
#include "ComponentPool.h"

class Transform;
class Mesh;
class Material;
class SceneGraph;
class RenderingSystem;

class Renderable : public Component
{
public:
	Renderable(Mesh* mesh, Material* mat) { this->mesh = mesh; this->material = mat; }
	void Destroy() { flagDestroy = true; }

	void SetMesh(Mesh* m) { mesh = m; }
	Mesh* GetMesh() { return mesh; }

	void SetMaterial(Material* mat) { material = mat; }
	Material* GetMaterial() { return material; }

	const DirectX::BoundingBox& BoundingBox() const { return bounds; }

private:
	friend class RenderingSystem;
	friend class SceneGraph;

	Renderable()
		:
		mesh(nullptr),
		material(nullptr),
		bounds(),
		flagDestroy(false)
	{}


	Mesh*					mesh;
	Material*				material;
	DirectX::BoundingBox	bounds;

	bool					flagDestroy;
};
