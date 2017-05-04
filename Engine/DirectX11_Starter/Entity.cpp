#include "Entity.h"
#include "ComponentManager.h"

using namespace std;

Entity::~Entity()
{
	ComponentManager::current->RemoveAllComponents(GetID());
}

Entity::Entity() {
	// 
	name = "Entity";
}

Entity::Entity(string name)
{
	this->name = name;
}

string Entity::GetName()
{
	return name;
}
