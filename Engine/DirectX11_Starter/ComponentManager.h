#pragma once
#include "Component.h"
#include <vector>
#include <utility>
#include "Entity.h"
#include "Transform.h"
#include "RigidBody.h"
#include "Renderable.h"
#include "ScriptComponent.h"
#include <map>
#include "ComponentPool.h"

typedef std::map<TypeId, std::vector<typePoolIndex *>> ComponentsMap;
typedef std::map<eidType, ComponentsMap> EntityComponentsMap;

class Scene;

class ComponentManager
{
public:
	ComponentManager();
	~ComponentManager();
	//void AddComponent(int entityID, Component* component, TypeId componentTypeId);

	template <typename T, typename... Args>
	T* AddComponent(int entityID, Args&&... args);

	template <typename T>
	bool RemoveComponent(int entityID);
	
	// TODO: const function
	template <typename T>
	T* GetComponent(int entityID);

	template <typename T>
	ResultComponents<T> GetAllComponent();

	//bool HasComponent(int entityID, TypeId componentTypeId) const;
	
	//std::vector<Transform*> transfromComponents;
	//std::vector<RigidBody*> rigidBodyComponents;
	//std::vector<Renderable*> renderables;
	//std::vector<ScriptComponent*> scriptComponents;
	
	//void Release();

	Transform* root;
	static ComponentManager* current;	

private:
	Scene* curScene;

	EntityComponentsMap entityComponentsMap;
	std::map<TypeId, ComponentPoolBase *> ComponentPoolsMap;

	static const int poolInitialSize = 100;
	static const int poolResizeAmount = 50;
};

template <typename T, typename... Args>
inline T* ComponentManager::AddComponent(int entityID, Args && ...args)
{
	//ComponentPool<T>* pool = reinterpret_cast<ComponentPool<T>*>(ComponentPoolsMap[ComponentTypeId<T>()]);

	ComponentPool<T>* pool;
	auto it = ComponentPoolsMap.find(ComponentTypeId<T>());

	// there's no such element in the map
	if (it == ComponentPoolsMap.end()) {
		pool = new ComponentPool<T>(poolInitialSize, poolResizeAmount, poolResizeAmount == 0 ? false : true);
		ComponentPoolsMap[ComponentTypeId<T>()] = pool;
	}
	else {
		pool = reinterpret_cast<ComponentPool<T>*>(it->second);
	}
		
	T *component = pool->AddComponent(std::forward<Args>(args)...);

	// FIXME: what to do?
	component->SetEntity(entityID);

	// http://www.cplusplus.com/reference/map/map/operator[]/
	entityComponentsMap[entityID][ComponentTypeId<T>()].push_back(component->getPoolIndex());
	//ComponentsMap * map = entityComponentsMap[entityID];

	return component;
}

template<typename T>
inline bool ComponentManager::RemoveComponent(int entityID)
{
	return false;
}

template<typename T>
inline T * ComponentManager::GetComponent(int entityID)
{
	std::vector<typePoolIndex *> indices = entityComponentsMap[entityID][ComponentTypeId<T>()];

	if (indices.size() == 0) {
		// For debug
		throw "No that component";
		return nullptr;
	}
	else {
		ComponentPool<T>* pool = reinterpret_cast<ComponentPool<T>*>(ComponentPoolsMap[ComponentTypeId<T>()]);
		return pool->GetComponent(*indices.front());
	}
}

template<typename T>
inline ResultComponents<T> ComponentManager::GetAllComponent()
{
	ComponentPool<T>* pool;
	auto it = ComponentPoolsMap.find(ComponentTypeId<T>());

	// there's no such element in the map
	if (it == ComponentPoolsMap.end()) {
		pool = new ComponentPool<T>(poolInitialSize, poolResizeAmount, poolResizeAmount == 0 ? false : true);
		ComponentPoolsMap[ComponentTypeId<T>()] = pool;
	}
	else {
		pool = reinterpret_cast<ComponentPool<T>*>(it->second);
	}

	return pool->GetAllComponents();
}