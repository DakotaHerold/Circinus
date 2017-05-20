#pragma once
#include "Component.h"
#include <vector>
#include <utility>
#include "Entity.h"
#include "Transform.h"
#include <map>
#include "ObjectPool.h"

class Scene;

// EntityID -> TypeID
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

	template <typename T, typename... Args>
	T* AddComponentWithoutPool(EntityID entityID, Args&&... args);
	
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

	static ComponentManager *current;

	void SetRoot(Transform *r) { root = r; }
	Transform* GetRoot() { return root; }

private:
	Scene* curScene;
	Transform *root = nullptr;

	EntityComponentsMap entityComponentsMap;
	std::map<TypeId, ObjectPoolBase *> ComponentPoolsMap;

	template<typename T>
	ObjectPool<T>* GetComponentPool();

	ObjectPoolBase* GetComponentPool(TypeId typeID);
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

template<typename T, typename ...Args>
inline T * ComponentManager::AddComponentWithoutPool(EntityID entityID, Args && ...args)
{
	T *component = new T(std::forward<Args>(args)...);

	// FIXME: what to do?
	component->SetEntity(entityID);

	entityComponentsMap[entityID][ComponentTypeId<T>()].push_back(0);
	entityComponentsMap[entityID][ComponentTypeId<T>()].pop_back();

	return component;
}

template<typename T>
inline T * ComponentManager::GetComponent(EntityID entityID, ObjectPoolIndex* index)
{
	std::vector<ObjectPoolIndex *> &indices = entityComponentsMap[entityID][ComponentTypeId<T>()];

	if (indices.size() == 0) {
		//#if defined(DEBUG) || defined(_DEBUG)
		//		throw "Entity did not have the component!";
		//#endif
		return nullptr;
	}

	if (index == nullptr) {
		index = indices.back();
	}
	else {
		auto it = std::remove_if(indices.begin(), indices.end(),
			[index](const ObjectPoolIndex *i) {
			return *i == *index;
		});

		if (it == indices.end()) {
			return nullptr;
		}
	}

	return reinterpret_cast<T*>(GetComponentPool<T>()->Get(*index));
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