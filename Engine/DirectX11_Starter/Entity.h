#pragma once

#include "Mesh.h"
#include "Material.h"
#include <utility>
#include <rapidjson\prettywriter.h>
#include <string>
#include <list>
#include "ClassTypeId.h"
#include <type_traits>

using namespace DirectX;
class ComponentManager;
class Component;
class Transform;

typedef unsigned int EntityID;

class Entity
{
public:
	Entity();
	Entity(std::string name, bool test);
	Entity(std::string name);
	Entity(std::string name, Material* mat, Mesh* mesh);

	~Entity();

	static Entity* GetEntity(EntityID eid);
	static const std::list<Entity*> GetAllEntities();

	EntityID GetID() const { return id; };

	template <typename T, typename... Args>
	T* AddComponent(Args&&... args);

	template <typename T>
	bool RemoveComponent();

	bool RemoveComponent(TypeId typeID);

	// TODO: const
	template <typename T>
	T* GetComponent();

	std::vector<Component *> GetAllComponents();

	//template <typename T>
	//bool				HasComponent() const;

	std::string GetName();
	
	void ChangeName(std::string name);

	void SetMaterial(std::string material) { this->material = material; }

	void SetMesh(std::string mesh) { this->mesh = mesh; }

	void Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer);

	Transform * const transform;

	bool test = false;

private :
	EntityID id;
	std::string name = "No Name Yet";
	std::string mesh ="";
	std::string material = "";
};

template <typename T, typename... Args>
T* Entity::AddComponent(Args&&... args)
{
	static_assert(std::is_base_of<Component, T>(), "T is not a component, cannot add T to entity");
	return ComponentManager::current->AddComponent<T>(GetID(), std::forward<Args>(args)...);
}

template <typename T>
bool Entity::RemoveComponent()
{
	static_assert(std::is_base_of<Component, T>(), "T is not a component, cannot remove T from entity");
	return RemoveComponent(ComponentTypeId<T>());
}

template <typename T>
T* Entity::GetComponent()
{
	static_assert(std::is_base_of<Component, T>(), "T is not a component, cannot retrieve T from entity");

	if (std::is_same<T, Transform>::value) {
		return reinterpret_cast<T*>(transform);
	}
	else {
		return ComponentManager::current->GetComponent<T>(GetID());
	}
}

//template <typename T>
//bool Entity::HasComponent() const
//{
//	static_assert(std::is_base_of<Component, T>(), "T is not a component, cannot determine if entity has T");
//	return HasComponent(ComponentTypeId<T>());
//}