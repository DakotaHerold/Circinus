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

Entity * Component::GetEntity()
{
	return gameEntity;
}

void Component::SetEntity(Entity * entity)
{
	gameEntity = entity;
}
