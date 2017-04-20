#pragma once

#include <d3d11.h>
#include "Mesh.h"
#include "Material.h"
#include "Component.h"
#include "Object.h"
#include <utility>
#include "Component.h"
#include "ComponentManager.h"

using namespace DirectX;
class ComponentManager;
class Entity : public Object
{
public:
	~Entity();
	Entity(); 


public :

	template <typename T, typename... Args>
	T*					AddComponent(Args&&... args);

	template <typename T>
	bool				RemoveComponent();

	template <typename T>
	T*					GetComponent() const;

	template <typename T>
	bool				HasComponent() const;

private :
	void				AddComponent(Component* component, TypeId componentTypeId);

	bool				RemoveComponent(TypeId componentTypeId);

	Component *			GetComponent(TypeId componentTypeId) const;

	bool				HasComponent(TypeId componentTypeId) const;
};

template <typename T, typename... Args>
T* Entity::AddComponent(Args&&... args)
{
	static_assert(std::is_base_of<Component, T>(), "T is not a component, cannot add T to entity");
	// TODO: align components by type
	auto component = new T{ std::forward<Args>(args)... };
	AddComponent(component, ComponentTypeId<T>());
	return component;
}

template <typename T>
bool Entity::RemoveComponent()
{
	static_assert(std::is_base_of<Component, T>(), "T is not a component, cannot remove T from entity");
	return RemoveComponent(ComponentTypeId<T>());
}

template <typename T>
T* Entity::GetComponent() const
{
	static_assert(std::is_base_of<Component, T>(), "T is not a component, cannot retrieve T from entity");
	return static_cast<T*>(GetComponent(ComponentTypeId<T>()));
}

template <typename T>
bool Entity::HasComponent() const
{
	static_assert(std::is_base_of<Component, T>(), "T is not a component, cannot determine if entity has T");
	return HasComponent(ComponentTypeId<T>());
}