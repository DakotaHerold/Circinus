#include "Component.h"
#include <iostream>

Component::Component()
{
}

Component::~Component()
{
}

void Component::Update()
{
	std::cout << "Base" << std::endl;
}

void Component::Release()
{
	this->~Component();
}

Entity* Component::GetEntity()
{
	return Object::GetObjectWithID<Entity>(gameEntityID);
}

int Component::GetEntityID()
{
	return gameEntityID;
}

void Component::SetEntity(int id)
{
	gameEntityID = id;
}
