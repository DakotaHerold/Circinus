#include "ComponentManager.h"
#include "Scene.h"


ComponentManager::ComponentManager()
{
	root = new Transform();
}


ComponentManager::~ComponentManager()
{

}




void ComponentManager::AddComponent(int entityID, Component* component, TypeId componentTypeId)
{
	//getWorld().m_entityAttributes.componentStorage.addComponent(*this, component, componentTypeId);

	//component->SetEntity(ent);
	component->SetEntity(entityID);
	if (componentTypeId == ComponentTypeId<Transform>()) {
		transfromComponents.push_back(dynamic_cast<Transform*>(component));
	}
	else if (componentTypeId == ComponentTypeId<RigidBody>()) {
		rigidBodyComponents.push_back(dynamic_cast<RigidBody*>(component));
	}
	else if (componentTypeId == ComponentTypeId<Renderable>()) {
		renderables.push_back(dynamic_cast<Renderable*>(component));
	}
	
}

bool ComponentManager::RemoveComponent(int entityID, TypeId componentTypeId)
{
	//getWorld().m_entityAttributes.componentStorage.removeComponent(*this, componentTypeId);
	if (componentTypeId == ComponentTypeId<Transform>()) {
		for (unsigned i = 0; i < transfromComponents.size(); i++) {
			if (transfromComponents[i]->GetEntityID() == entityID) {
				transfromComponents.erase(transfromComponents.begin() + i);
				return true;
			}
		}
	}
	else if (componentTypeId == ComponentTypeId<RigidBody>()) {
		for (unsigned i = 0; i < rigidBodyComponents.size(); i++) {
			if (rigidBodyComponents[i]->GetEntityID() == entityID) {
				rigidBodyComponents.erase(rigidBodyComponents.begin() + i);
				return true;
			}
		}
	}
	else if (componentTypeId == ComponentTypeId<Renderable>()) {
		for (unsigned i = 0; i < renderables.size(); i++) {
			if (renderables[i]->GetEntityID() == entityID) {
				renderables.erase(renderables.begin() + i);
				return true;
			}
		}
	}
	
	return false;
}

Component * ComponentManager::GetComponent(int entityID, TypeId componentTypeId) const
{
	if (componentTypeId == ComponentTypeId<Transform>()) {
		for (auto p : transfromComponents) {
			if (p->GetEntityID() == entityID) {
				return p;
			}
		}
	}
	else if (componentTypeId == ComponentTypeId<RigidBody>()) {
		for (auto p : rigidBodyComponents) {
			if (p->GetEntityID() == entityID) {
				return p;
			}
		}
	}
	else if (componentTypeId == ComponentTypeId<Renderable>()) {
		for (auto p : renderables) {
			if (p->GetEntityID() == entityID) {
				return p;
			}
		}
	}
	return nullptr;

	//return getWorld().m_entityAttributes.componentStorage.getComponent(*this, componentTypeId);
}

bool ComponentManager::HasComponent(int entityID, TypeId componentTypeId) const
{
	if (componentTypeId == ComponentTypeId<Transform>()) {
		for (auto p : transfromComponents) {
			if (p->GetEntityID() == entityID) {
				return true;
			}
		}
	}
	else if (componentTypeId == ComponentTypeId<RigidBody>()) {
		for (auto p : rigidBodyComponents) {
			if (p->GetEntityID() == entityID) {
				return true;
			}
		}
	}
	else if (componentTypeId == ComponentTypeId<Renderable>()) {
		for (auto p : renderables) {
			if (p->GetEntityID() == entityID) {
				return true;
			}
		}
	}
	return false;
}

void ComponentManager::Release()
{
	for (int i = 0; i < transfromComponents.size(); i++) {
		delete transfromComponents[i];
	}
	transfromComponents.clear();

	for (int i = 0; i < rigidBodyComponents.size(); i++) {
		delete rigidBodyComponents[i];
	}
	rigidBodyComponents.clear();

	for (int i = 0; i < renderables.size(); i++) {
		delete renderables[i];
	}
	renderables.clear();
	this->~ComponentManager();
}



ComponentManager* ComponentManager::current;
