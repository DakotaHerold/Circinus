#pragma once

#include <DirectXMath.h>
#include <DirectXCollision.h>
#include "Component.h"

class Transform;
class Mesh;
class Material;
class SceneGraph;
class RenderingSystem;

class Renderable : public Component
{
public:
	Renderable(Mesh* mesh, Material* mat) : Renderable() { this->mesh = mesh; this->material = mat; bounds = mesh->GetBounds(); }

	Renderable()
		:
		mesh(nullptr),
		material(nullptr),
		bounds(),
		flagDestroy(false)
	{}

	void Init(Mesh* mesh, Material* mat) { this->mesh = mesh; this->material = mat; bounds = mesh->GetBounds(); }
	void Destroy() { flagDestroy = true; }

	void SetMesh(Mesh* m) { mesh = m; bounds = mesh->GetBounds(); }
	Mesh* GetMesh() { return mesh; }

	void SetMaterial(Material* mat) { material = mat; }
	Material* GetMaterial() { return material; }

	// local space AABB boundingbox
	DirectX::BoundingBox& BoundingBox() { return bounds; }

	void Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) {
		writer.StartObject();
		writer.String("name");
		writer.String("Renderable");
		writer.EndObject();
	}

private:
	friend class RenderingSystem;
	friend class Scene;
#ifdef HAS_GUI
	friend class GUI;
#endif



	Mesh*					mesh;
	Material*				material;

	// local space AABB boundingbox
	DirectX::BoundingBox	bounds;

	bool					flagDestroy;
};
