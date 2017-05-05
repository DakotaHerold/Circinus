#include "Component.h"
#include "Entity.h"

Component::Component()
{
	
}

Component::~Component()
{
	
}

void Component::Update()
{

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
