#include "ComponentManager.h"
#include "Scene.h"

using namespace std;

ComponentManager* ComponentManager::current;
//ComponentManager* ComponentManager::current = new ComponentManager();

ComponentManager::ComponentManager()
{
	root = new Transform();
}

ComponentManager::~ComponentManager()
{
	delete root;

	for (auto i : ComponentPoolsMap) {
		delete i.second;
	}
}

vector<pair<TypeId, typePoolIndex *>> ComponentManager::GetAllComponents(int entityID)
{
	vector<pair<TypeId, typePoolIndex*>> result;

	for (auto p : entityComponentsMap[entityID]) {
		for (typePoolIndex *i : p.second) {
			result.push_back(make_pair(p.first, i));
		}
	}

	return result;
}

//void ComponentManager::AddComponent(int entityID, Component* component, TypeId componentTypeId)
//{
//	//getWorld().m_entityAttributes.componentStorage.addComponent(*this, component, componentTypeId);
//
//	//component->SetEntity(ent);
//	component->SetEntity(entityID);
//	if (componentTypeId == ComponentTypeId<Transform>()) {
//		transfromComponents.push_back(dynamic_cast<Transform*>(component));
//	}
//	else if (componentTypeId == ComponentTypeId<RigidBody>()) {
//		rigidBodyComponents.push_back(dynamic_cast<RigidBody*>(component));
//	}
//	else if (componentTypeId == ComponentTypeId<Renderable>()) {
//		renderables.push_back(dynamic_cast<Renderable*>(component));
//	}
//	else if (componentTypeId == ComponentTypeId<ScriptComponent>()) {
//		scriptComponents.push_back(dynamic_cast<ScriptComponent*>(component));
//	}
//}

//bool ComponentManager::RemoveComponent(int entityID, TypeId componentTypeId)
//{
//	//getWorld().m_entityAttributes.componentStorage.removeComponent(*this, componentTypeId);
//	if (componentTypeId == ComponentTypeId<Transform>()) {
//		for (unsigned i = 0; i < transfromComponents.size(); i++) {
//			if (transfromComponents[i]->GetEntityID() == entityID) {
//				transfromComponents.erase(transfromComponents.begin() + i);
//				return true;
//			}
//		}
//	}
//	else if (componentTypeId == ComponentTypeId<RigidBody>()) {
//		for (unsigned i = 0; i < rigidBodyComponents.size(); i++) {
//			if (rigidBodyComponents[i]->GetEntityID() == entityID) {
//				rigidBodyComponents.erase(rigidBodyComponents.begin() + i);
//				return true;
//			}
//		}
//	}
//	else if (componentTypeId == ComponentTypeId<Renderable>()) {
//		for (unsigned i = 0; i < renderables.size(); i++) {
//			if (renderables[i]->GetEntityID() == entityID) {
//				renderables.erase(renderables.begin() + i);
//				return true;
//			}
//		}
//	}
//	else if (componentTypeId == ComponentTypeId<ScriptComponent>()) {
//		for (unsigned i = 0; i < renderables.size(); i++) {
//			if (scriptComponents[i]->GetEntityID() == entityID) {
//				scriptComponents.erase(scriptComponents.begin() + i);
//				return true;
//			}
//		}
//	}
//	
//	return false;
//}
//
//Component * ComponentManager::GetComponent(int entityID, TypeId componentTypeId) const
//{
//	if (componentTypeId == ComponentTypeId<Transform>()) {
//		for (auto p : transfromComponents) {
//			if (p->GetEntityID() == entityID) {
//				return p;
//			}
//		}
//	}
//	else if (componentTypeId == ComponentTypeId<RigidBody>()) {
//		for (auto p : rigidBodyComponents) {
//			if (p->GetEntityID() == entityID) {
//				return p;
//			}
//		}
//	}
//	else if (componentTypeId == ComponentTypeId<Renderable>()) {
//		for (auto p : renderables) {
//			if (p->GetEntityID() == entityID) {
//				return p;
//			}
//		}
//	}
//	else if (componentTypeId == ComponentTypeId<ScriptComponent>()) {
//		for (auto p : scriptComponents) {
//			if (p->GetEntityID() == entityID) {
//				return p;
//			}
//		}
//	}
//
//
//	return nullptr;
//
//	//return getWorld().m_entityAttributes.componentStorage.getComponent(*this, componentTypeId);
//}

//bool ComponentManager::HasComponent(int entityID, TypeId componentTypeId) const
//{
//	if (componentTypeId == ComponentTypeId<Transform>()) {
//		for (auto p : transfromComponents) {
//			if (p->GetEntityID() == entityID) {
//				return true;
//			}
//		}
//	}
//	else if (componentTypeId == ComponentTypeId<RigidBody>()) {
//		for (auto p : rigidBodyComponents) {
//			if (p->GetEntityID() == entityID) {
//				return true;
//			}
//		}
//	}
//	else if (componentTypeId == ComponentTypeId<Renderable>()) {
//		for (auto p : renderables) {
//			if (p->GetEntityID() == entityID) {
//				return true;
//			}
//		}
//	}
//	else if (componentTypeId == ComponentTypeId<ScriptComponent>()) {
//		for (auto p : scriptComponents) {
//			if (p->GetEntityID() == entityID) {
//				return true;
//			}
//		}
//	}
//	return false;
//}
//
//void ComponentManager::Release()
//{
//	for (int i = 0; i < transfromComponents.size(); i++) {
//		delete transfromComponents[i];
//	}
//	transfromComponents.clear();
//
//	for (int i = 0; i < rigidBodyComponents.size(); i++) {
//		delete rigidBodyComponents[i];
//	}
//	rigidBodyComponents.clear();
//
//	for (int i = 0; i < renderables.size(); i++) {
//		delete renderables[i];
//	}
//	renderables.clear();
//
//	for (int i = 0; i < scriptComponents.size(); i++) {
//		delete scriptComponents[i];
//	}
//
//	scriptComponents.clear();
//
//	this->~ComponentManager();
//}