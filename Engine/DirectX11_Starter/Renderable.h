#pragma once

#include <DirectXMath.h>
#include <DirectXCollision.h>

class Transform;
class Mesh;
class Material;
class SceneGraph;
class RenderingSystem;

class Renderable
{
public:
	
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
		parent(nullptr),
		transform(nullptr),
		mesh(nullptr),
		material(nullptr),
		bounds(),
		flagDestroy(false)
	{}

	Renderable*				parent;

	Transform*				transform;
	Mesh*					mesh;
	Material*				material;
	DirectX::BoundingBox	bounds;

	bool					flagDestroy;
};
