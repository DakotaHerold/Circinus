#include "Entity.h"
#include "ComponentManager.h"

using namespace std;

Entity::~Entity()
{
	ComponentManager::current->RemoveAllComponents(GetID());
}

Entity::Entity() {
	// 
}

string Entity::GetName()
{
	return name;
}
