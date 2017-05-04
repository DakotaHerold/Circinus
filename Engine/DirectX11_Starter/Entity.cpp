#include "Entity.h"
#include "ComponentManager.h"

using namespace std;

Entity::Entity() {

}

Entity::~Entity()
{
	ComponentManager::current->RemoveAllComponents(GetID());
}

string Entity::GetName()
{
	return name;
}
