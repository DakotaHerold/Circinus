#include "Entity.h"
#include "ComponentManager.h"

using namespace std;

Entity::~Entity()
{
	//delete components
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

//void Entity::AddComponent(Component* component, TypeId componentTypeId)
//{
//	//getWorld().m_entityAttributes.componentStorage.addComponent(*this, component, componentTypeId);
//	//ComponentManager::current->AddComponent(GetID(), component, componentTypeId);
//}

//bool Entity::RemoveComponent(TypeId componentTypeId)
//{
//	//getWorld().m_entityAttributes.componentStorage.removeComponent(*this, componentTypeId);
//	//return ComponentManager::current->RemoveComponent(GetID(), componentTypeId);
//	return false;
//}

//Component * Entity::GetComponent(TypeId componentTypeId) const
//{
//	//return getWorld().m_entityAttributes.componentStorage.getComponent(*this, componentTypeId);
//	return (ComponentManager::current->GetComponent(GetID(), componentTypeId));
//}

//bool Entity::HasComponent(TypeId componentTypeId) const
//{
//	return ComponentManager::current->HasComponent(GetID(), componentTypeId);
//}

//void Entity::RemoveAllComponents()
//{
//	/*getWorld().m_entityAttributes.componentStorage.removeAllComponents(*this);*/
//}

