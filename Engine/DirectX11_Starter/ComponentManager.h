#pragma once
#include "Component.h"
#include <vector>
#include <utility>
#include "Entity.h"
#include "Transform.h"
#include <map>
#include "ObjectPool.h"

class Scene;

typedef std::map<TypeId, std::vector<ObjectPoolIndex *>> ComponentsMap;
typedef std::map<eidType, ComponentsMap> EntityComponentsMap;

class ComponentManager
{
public:
	ComponentManager();
	~ComponentManager();

	template <typename T, typename... Args>
	T* AddComponent(int entityID, Args&&... args);
	
	// TODO: const function
	template <typename T>
	T* GetComponent(int entityID);

	template <typename T>
	T* GetComponent(int entityID, ObjectPoolIndex *index);

	template <typename T>
	bool RemoveComponent(int entityID);

	template <typename T>
	bool RemoveComponent(int entityID, ObjectPoolIndex *index);

	//bool HasComponent(int entityID, TypeId componentTypeId) const;

	template <typename T>
	ResultComponents<T> GetAllComponents();

	std::vector<std::pair<TypeId, ObjectPoolIndex *>> GetAllComponents(int entityID);

	bool RemoveComponentWithIndex(int entityID, TypeId typeID, ObjectPoolIndex *index = nullptr);

	void RemoveAllComponents(int entityID);

	Transform *root;
	static ComponentManager *current;	

private:
	Scene* curScene;

	EntityComponentsMap entityComponentsMap;
	std::map<TypeId, ObjectPoolBase *> ComponentPoolsMap;

	template<typename T>
	ObjectPool<T>* GetComponentPool();

	ObjectPoolBase* GetComponentPool(TypeId typeID);

	int getObjectIndex(eidType entityID, TypeId typeID, ObjectPoolIndex *index = nullptr, bool deleteIndex = false);
};

template<typename T>
inline ObjectPool<T>* ComponentManager::GetComponentPool()
{
	auto it = ComponentPoolsMap.find(ComponentTypeId<T>());

	ObjectPool<T> *pool = reinterpret_cast<ObjectPool<T>*>(GetComponentPool(ComponentTypeId<T>()));

	// make a pool
	if (pool == nullptr) {
		pool = new ObjectPool<T>();
		ComponentPoolsMap[ComponentTypeId<T>()] = pool;
	}
	
	return pool;
}

template <typename T, typename... Args>
inline T* ComponentManager::AddComponent(int entityID, Args && ...args)
{	
	T *component = GetComponentPool<T>()->Add(std::forward<Args>(args)...);

	// FIXME: what to do?
	component->SetEntity(entityID);

	// http://www.cplusplus.com/reference/map/map/operator[]/
	entityComponentsMap[entityID][ComponentTypeId<T>()].push_back(component->getPoolIndex());
	
	return component;
}

template<typename T>
inline T * ComponentManager::GetComponent(int entityID)
{
	return GetComponent<T>(entityID, getObjectIndex(entityID, ComponentTypeId<T>()));
}

template<typename T>
inline T * ComponentManager::GetComponent(int entityID, ObjectPoolIndex* index)
{
	return GetComponentPool<T>()->Get(*index);
}

template<typename T>
inline bool ComponentManager::RemoveComponent(int entityID)
{
	return RemoveComponentWithIndex(entityID, ComponentTypeId<T>());
}

template<typename T>
inline bool ComponentManager::RemoveComponent(int entityID, ObjectPoolIndex* index)
{
	return RemoveComponentWithIndex(entityID, ComponentTypeId<T>(), index);
}

template<typename T>
inline ResultComponents<T> ComponentManager::GetAllComponents()
{
	return GetComponentPool<T>()->GetAllComponents();
}