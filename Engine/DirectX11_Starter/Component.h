#pragma once
#include "ClassTypeId.h"

class Entity;
class Component
{
public:
	Component();
	~Component();
	
	virtual void Update();
	virtual void Release();

	Entity* GetEntity();
	virtual void SetEntity(Entity* entity);

protected:
	Entity* gameEntity;
};

template <typename T>
TypeId ComponentTypeId()
{
	return ClassTypeId<Component>::GetTypeId<T>();
}
