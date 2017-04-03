#include "Component.h"



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


Entity * Component::GetEntity()
{
	return gameEntity;
}

ComponentType Component::GetType()
{
	return type;
}



void Component::SetEntity(Entity * entity)
{
	gameEntity = entity;
}
