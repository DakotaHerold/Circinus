#pragma once
#include "ClassTypeId.h"
#include "Object.h"
#include <utility>
class Entity;
class Component : public Object
{
	template <typename U>
	friend class ComponentPool;

public:
	Component();
	~Component();

	
	
	virtual void				Update();

	virtual void				Release();

	virtual void				SetEntity(int id);
	Entity*						GetEntity();
	int							GetEntityID();


private:
	int							gameEntityID;
	int							poolIndex;
};

template <typename T>
TypeId ComponentTypeId()
{
	return ClassTypeId<Component>::GetTypeId<T>();
}


