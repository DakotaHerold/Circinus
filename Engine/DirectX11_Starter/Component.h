#pragma once
#include<string>
#include <iostream>
#include "ComponentType.h"
class Entity;
class Component
{
public:
	Component();
	~Component();
	
	virtual void Update();
	virtual void Release();

	Entity* GetEntity();
	ComponentType GetType();
	void SetEntity(Entity* entity);
protected:
	Entity* gameEntity;
	ComponentType type;
	
};

