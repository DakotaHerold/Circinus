#include "Component.h"

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
	return (Entity::GetEntity(gameEntityID));
}

int Component::GetEntityID()
{
	return gameEntityID;
}

void Component::SetEntity(EntityID eid)
{
	gameEntityID = eid;
}
