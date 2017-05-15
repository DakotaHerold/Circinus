#pragma once
#include "ClassTypeId.h"
#include <utility>
#include "ObjectPool.h"
#include "Entity.h"

class Component : public Poolable
{
public:
	Component();
	~Component();

	virtual void				Update();

	virtual void				SetEntity(EntityID eid);
	Entity*						GetEntity();
	int							GetEntityID();

private:
	EntityID 					gameEntityID;
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


