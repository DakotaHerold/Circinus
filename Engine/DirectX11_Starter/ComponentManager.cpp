#include "ComponentManager.h"
#include "Scene.h"
#include <algorithm>

ComponentManager* ComponentManager::current;
//ComponentManager* ComponentManager::current = new ComponentManager();

static ObjectPoolIndex nullObjectIndex = UINT_MAX;

ComponentManager::ComponentManager()
{
	root = new Transform();
	root->UpdateTransform();
}

ComponentManager::~ComponentManager()
{
	delete root;

	for (auto i : ComponentPoolsMap) {
		delete i.second;
	}
}

vector<Component*> ComponentManager::GetAllComponents(EntityID entityID)
{
	vector<Component*> ret;

	for (auto &p : entityComponentsMap[entityID]) {
		ObjectPoolBase *pool = GetComponentPool(p.first);

		for (ObjectPoolIndex *i : p.second) {
			ret.push_back(reinterpret_cast<Component*>(pool->Get(*i)));
		}
	}

	return ret;
}

vector<pair<TypeId, ObjectPoolIndex *>> ComponentManager::GetAllComponentsInfo(EntityID entityID)
{
	vector<pair<TypeId, ObjectPoolIndex*>> result;

	for (auto &p : entityComponentsMap[entityID]) {
		for (ObjectPoolIndex *i : p.second) {
			result.push_back(make_pair(p.first, i));
		}
	}

	return result;
}

bool ComponentManager::RemoveComponent(EntityID entityID, TypeId typeID, ObjectPoolIndex *index)
{
	std::vector<ObjectPoolIndex *> &indices = entityComponentsMap[entityID][typeID];

	if (indices.size() == 0) {
		//#if defined(DEBUG) || defined(_DEBUG)
		//		throw "Entity did not have the component!";
		//#endif
		return false;
	}

	if (index == nullptr) {
		index = indices.back();
		indices.pop_back();
	}
	else {
		auto it = std::remove_if(indices.begin(), indices.end(),
			[index](const ObjectPoolIndex *i) {
			return *i == *index;
		});

		if (it == indices.end()) {
			return false;
		}

		indices.erase(it, indices.end());
	}

	return GetComponentPool(typeID)->Return(*index);
}

void ComponentManager::RemoveAllComponents(EntityID entityID)
{
	for (auto &p : entityComponentsMap[entityID]) {
		for (ObjectPoolIndex *i : p.second) {
			GetComponentPool(p.first)->Return(*i);
		}

		// TODO: clear entity component map?
		p.second.clear();
	}
}


ObjectPoolBase * ComponentManager::GetComponentPool(TypeId typeID)
{
	auto it = ComponentPoolsMap.find(typeID);

	if (it == ComponentPoolsMap.end()) {
		return nullptr;
	}
	else {
		return it->second;
	}
}