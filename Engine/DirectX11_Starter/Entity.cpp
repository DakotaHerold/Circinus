#include "Entity.h"
#include "ComponentManager.h"

using namespace std;

static EntityID nextID = 0;

//TODO: delete and use entity manager
static vector<Entity*> entities;

// alive entity list
static list<Entity*> allEntities;

Entity::Entity() 
	: name("Entity"), id(nextID++)
{
	entities.push_back(this);
	allEntities.push_back(this);
}

Entity::Entity(string name)
	: name(name), id(nextID++)
{
	entities.push_back(this);
	allEntities.push_back(this);
}

Entity::~Entity()
{
	ComponentManager::current->RemoveAllComponents(GetID());
	allEntities.remove(this);
}

Entity * Entity::GetEntity(EntityID eid)
{
	if (eid >= nextID)
		throw "ID is invalid, too big!";

	return entities[eid];
}

const list<Entity*> Entity::GetAllEntities()
{
	return allEntities;
}

bool Entity::RemoveComponent(TypeId typeID)
{
	return ComponentManager::current->RemoveComponent(GetID(), typeID);
}

string Entity::GetName()
{
	return name;
}

std::vector<Component*> Entity::GetAllComponents()
{
	return ComponentManager::current->GetAllComponents(GetID());
}

void Entity::ChangeName(std::string name)
{
	this->name = name;
}
