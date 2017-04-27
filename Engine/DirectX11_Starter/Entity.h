#pragma once

#include <d3d11.h>
#include "Mesh.h"
#include "Material.h"
#include "Component.h"
#include "Object.h"
#include <utility>

using namespace DirectX;
class ComponentManager;

typedef int eidType;

class Entity : public Object
{
public:
	~Entity();
	Entity();
	Entity(string name);

public :

	template <typename T, typename... Args>
	T*					AddComponent(Args&&... args);

	template <typename T>
	bool				RemoveComponent();

	// TODO: const
	template <typename T>
	T*					GetComponent();

	template <typename T>
	bool				HasComponent() const;

	string GetName();

private :
	//void				AddComponent(Component* component, TypeId componentTypeId);

	//bool				RemoveComponent(TypeId componentTypeId);

	//Component *			GetComponent(TypeId componentTypeId) const;

	//bool				HasComponent(TypeId componentTypeId) const;

	string name;
};

template <typename T, typename... Args>
T* Entity::AddComponent(Args&&... args)
{
	static_assert(std::is_base_of<Component, T>(), "T is not a component, cannot add T to entity");
	return ComponentManager::current->AddComponent<T>(GetID(), std::forward<Args>(args)...);
}

//template <typename T>
//bool Entity::RemoveComponent()
//{
//	static_assert(std::is_base_of<Component, T>(), "T is not a component, cannot remove T from entity");
//	return RemoveComponent(ComponentTypeId<T>());
//}

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