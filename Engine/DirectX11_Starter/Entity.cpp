#include "Entity.h"
#include "ComponentManager.h"
#include "Renderable.h"
using namespace std;

Entity::Entity() 
	: name("Entity")
{
	AddComponent<Transform>();
}

Entity::Entity(string name)
	: name(name)
{
	AddComponent<Transform>();
}

Entity::Entity(std::string name, Material * mat, Mesh * mesh)
{
	this->name = name;
	AddComponent<Renderable>(mesh, mat);
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

void Entity::ChangeName(std::string name)
{
	this->name = name;
}
