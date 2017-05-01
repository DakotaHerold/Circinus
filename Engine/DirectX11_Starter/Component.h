#pragma once
#include "ClassTypeId.h"
#include "Object.h"
#include <utility>
#include "ComponentPool.h"

class Entity;

class Component : public Object
{
	template <typename U>
	friend class ComponentPool;

public:
	Component();
	~Component();

	virtual void				Update();

	virtual void				SetEntity(int id);
	Entity*						GetEntity();
	int							GetEntityID();

	typePoolIndex*				getPoolIndex() { return poolIndex; }

private:
	int							gameEntityID;

	typePoolIndex*				poolIndex = nullptr;
};

template <typename T>
TypeId ComponentTypeId()
{
	return ClassTypeId<Component>::GetTypeId<T>();
}

static char * ComponentTypeName(TypeId id)
{
	return ClassTypeId<Component>::GetTypeName(id);
}


