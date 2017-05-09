#include "Entity.h"
#include "ComponentManager.h"

using namespace std;

Entity::Entity() 
	: name("Entity")
{

}

Entity::Entity(string name)
	: name(name)
{

}

Entity::~Entity()
{
	ComponentManager::current->RemoveAllComponents(GetID());
}

bool Entity::RemoveComponent(TypeId typeID)
{
	return ComponentManager::current->RemoveComponent(GetID(), typeID);
}

string Entity::GetName()
{
	return name;
}
