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
	if (!hasEntity) {
		return -1;
	}
	return gameEntityID;
}

void Component::SetEntity(EntityID eid)
{
	gameEntityID = eid;
	hasEntity = true;
}
