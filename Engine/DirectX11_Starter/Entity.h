#pragma once

#include "Mesh.h"
#include "Material.h"
#include "Component.h"
#include "Object.h"
#include <utility>
#include <string>

using namespace DirectX;
class ComponentManager;

typedef int eidType;

class Entity : public Object
{
public:
	~Entity();
	Entity();
	Entity(std::string name);

public :

	template <typename T, typename... Args>
	T*					AddComponent(Args&&... args);

	template <typename T>
	bool				RemoveComponent();

	bool RemoveComponent(TypeId typeID);

	// TODO: const
	template <typename T>
	T*					GetComponent();

	//template <typename T>
	//bool				HasComponent() const;

	std::string GetName();

private :
	std::string name = "No Name Yet";
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
	return ComponentManager::current->GetComponent<T>(GetID());
}

//template <typename T>
//bool Entity::HasComponent() const
//{
//	static_assert(std::is_base_of<Component, T>(), "T is not a component, cannot determine if entity has T");
//	return HasComponent(ComponentTypeId<T>());
//}