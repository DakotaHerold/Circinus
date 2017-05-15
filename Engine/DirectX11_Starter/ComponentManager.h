#pragma once
#include "Component.h"
#include <vector>
#include <utility>
#include "Entity.h"
#include "Transform.h"
#include <map>
#include "ObjectPool.h"

class Scene;

//typedef std::vector<std::vector<std::vector<ObjectPoolIndex *>>> EntityComponentsIndices;

typedef std::map<TypeId, std::vector<ObjectPoolIndex *>> ComponentsMap;
typedef std::map<EntityID, ComponentsMap> EntityComponentsMap;

class ComponentManager
{
public:
	ComponentManager();
	~ComponentManager();

	template <typename T, typename... Args>
	T* AddComponent(EntityID entityID, Args&&... args);
	
	// TODO: const function
	template <typename T>
	T* GetComponent(EntityID entityID, ObjectPoolIndex *index = nullptr);

	//template <typename T>
	//bool RemoveComponent(int entityID, ObjectPoolIndex *index = nullptr);

	//bool HasComponent(int entityID, TypeId componentTypeId) const;

	template <typename T>
	std::vector<T*> GetAllComponents();

	std::vector<Component *> GetAllComponents(EntityID entityID);

	std::vector<std::pair<TypeId, ObjectPoolIndex *>> GetAllComponentsInfo(EntityID entityID);

	bool RemoveComponent(EntityID entityID, TypeId typeID, ObjectPoolIndex *index = nullptr);

	void RemoveAllComponents(EntityID entityID);

	Transform *root;
	static ComponentManager *current;	

private:
	Scene* curScene;

	EntityComponentsMap entityComponentsMap;
	std::map<TypeId, ObjectPoolBase *> ComponentPoolsMap;

	template<typename T>
	ObjectPool<T>* GetComponentPool();

	ObjectPoolBase* GetComponentPool(TypeId typeID);

	int GetObjectPoolIndex(EntityID entityID, TypeId typeID, bool deleteIndex = false);
	bool CheckObjectPoolIndex(EntityID entityID, TypeId typeID, ObjectPoolIndex * index, bool deleteIndex = false);
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
inline T* ComponentManager::AddComponent(EntityID entityID, Args && ...args)
{	
	T *component = GetComponentPool<T>()->Add(std::forward<Args>(args)...);

	// FIXME: what to do?
	component->SetEntity(entityID);

	// http://www.cplusplus.com/reference/map/map/operator[]/
	entityComponentsMap[entityID][ComponentTypeId<T>()].push_back(component->getPoolIndex());

	return component;
}

template<typename T>
inline T * ComponentManager::GetComponent(EntityID entityID, ObjectPoolIndex* index)
{
	if (index == nullptr) {
		return reinterpret_cast<T*>(GetComponentPool<T>()->Get(GetObjectPoolIndex(entityID, ComponentTypeId<T>())));
	}
	else {
		if (!CheckObjectPoolIndex(entityID, ComponentTypeId<T>(), index)) {
			return nullptr;
		}

		return reinterpret_cast<T*>(GetComponentPool<T>()->Get(*index));
	}
}

//template<typename T>
//inline bool ComponentManager::RemoveComponent(int entityID, ObjectPoolIndex* index)
//{
//	return RemoveComponent(entityID, ComponentTypeId<T>(), index);
//}

template<typename T>
inline std::vector<T*> ComponentManager::GetAllComponents()
{
	return GetComponentPool<T>()->GetAllComponents();
}