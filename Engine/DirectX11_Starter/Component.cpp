#include "Component.h"
#include "Entity.h"

Component::Component()
{
	poolIndex = -1;
}

Component::~Component()
{
}

void Component::Update()
{

}

void Component::Release()
{
	this->~Component();
}

Entity* Component::GetEntity()
{	
	return (Object::GetObjectWithID<Entity>(gameEntityID));
}

int Component::GetEntityID()
{
	return gameEntityID;
}

void Component::SetEntity(int id)
{
	gameEntityID = id;
}
