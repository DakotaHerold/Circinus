#pragma once
#include "Component.h"
#include <vector>
#include <utility>
#include "Entity.h"
#include "Transform.h"
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

	template <typename T, typename... Args>
	T* AddComponent(int entityID, Args&&... args);

	template <typename T>
	bool RemoveComponent(int entityID);
	
	// TODO: const function
	template <typename T>
	T* GetComponent(int entityID);

	template <typename T>
	T* GetComponent(int entityID, typePoolIndex index);

	template <typename T>
	ResultComponents<T> GetAllComponents();

	std::vector<std::pair<TypeId, typePoolIndex *>> GetAllComponents(int entityID);

	//bool HasComponent(int entityID, TypeId componentTypeId) const;

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
	std::vector<typePoolIndex *> indices = entityComponentsMap[entityID][ComponentTypeId<T>()];

	if (indices.size() == 0) {
#if defined(DEBUG) || defined(_DEBUG)
		throw "No that component";
#endif
		return false;
	}
	else {
		ComponentPool<T>* pool = reinterpret_cast<ComponentPool<T>*>(ComponentPoolsMap[ComponentTypeId<T>()]);
		pool->ReturnComponent(pool->GetComponent(index));
		indices.pop_back();
	}
}

template<typename T>
inline T * ComponentManager::GetComponent(int entityID)
{
	std::vector<typePoolIndex *> indices = entityComponentsMap[entityID][ComponentTypeId<T>()];

	if (indices.size() == 0) {
#if defined(DEBUG) || defined(_DEBUG)
		throw "No that component";
#endif
		return nullptr;
	}
	else {
		return GetComponent<T>(entityID, *(indices.front()));
	}
}

template<typename T>
inline T * ComponentManager::GetComponent(int entityID, typePoolIndex index)
{
	ComponentPool<T>* pool = reinterpret_cast<ComponentPool<T>*>(ComponentPoolsMap[ComponentTypeId<T>()]);
	return pool->GetComponent(index);
}

template<typename T>
inline ResultComponents<T> ComponentManager::GetAllComponents()
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