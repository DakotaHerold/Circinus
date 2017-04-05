#include "ComponentManager.h"



ComponentManager::ComponentManager()
{
}


ComponentManager::~ComponentManager()
{
}


void ComponentManager::NewComManager()
{
	ComponentManager* cm = new ComponentManager();
	ComponentManager::current = cm;
}

void ComponentManager::AddComponent(int entityID, Component* component, TypeId componentTypeId)
{
	//getWorld().m_entityAttributes.componentStorage.addComponent(*this, component, componentTypeId);

	//component->SetEntity(ent);
	component->SetEntity(entityID);
	allComponets.push_back(std::make_pair(componentTypeId, component));
}

bool ComponentManager::RemoveComponent(int entityID, TypeId componentTypeId)
{
	//getWorld().m_entityAttributes.componentStorage.removeComponent(*this, componentTypeId);

	for (int i = 0; i < allComponets.size(); i++) {
		if (allComponets[i].second->GetEntityID == entityID && allComponets[i].first == componentTypeId) {
			allComponets.erase(allComponets.begin() + i);
			return true;
		}
	}
	return false;
}

Component * ComponentManager::GetComponent(int entityID, TypeId componentTypeId) const
{
	for (auto p : allComponets) {
		if (p.second->GetEntityID() == entityID && p.first == componentTypeId) {
			return p.second;
		}
	}
	return nullptr;

	//return getWorld().m_entityAttributes.componentStorage.getComponent(*this, componentTypeId);
}

bool ComponentManager::HasComponent(int entityID, TypeId componentTypeId) const
{
	for (auto p : allComponets) {
		if (p.second->GetEntityID() == entityID && p.first == componentTypeId) {
			return true;
		}
	}
	return false;
}

ComponentManager * ComponentManager::GetCurrent()
{
	return current;
}

ComponentManager* ComponentManager::current;
