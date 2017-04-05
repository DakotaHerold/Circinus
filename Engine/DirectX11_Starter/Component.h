#pragma once
#include "ClassTypeId.h"
#include "Object.h"
class Entity;
class Component : public Object
{
public:
	Component();
	~Component();
	
	virtual void Update();
	virtual void Release();

	void SetEntity(int id);
	Entity* GetEntity();
	int GetEntityID();

private:
	int gameEntityID;
};

template <typename T>
TypeId ComponentTypeId()
{
	return ClassTypeId<Component>::GetTypeId<T>();
}
