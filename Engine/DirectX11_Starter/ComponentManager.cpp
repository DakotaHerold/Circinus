#include "ComponentManager.h"
#include "Scene.h"
#include <algorithm>

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

vector<pair<TypeId, ObjectPoolIndex *>> ComponentManager::GetAllComponents(int entityID)
{
	vector<pair<TypeId, ObjectPoolIndex*>> result;

	for (auto p : entityComponentsMap[entityID]) {
		for (ObjectPoolIndex *i : p.second) {
			result.push_back(make_pair(p.first, i));
		}
	}

	return result;
}

bool ComponentManager::RemoveComponent(int entityID, TypeId typeID, ObjectPoolIndex *index)
{
	if (index == nullptr) {
		GetComponentPool(typeID)->Return(GetObjectPoolIndex(entityID, typeID, true));
	}
	else {
		if (!CheckObjectPoolIndex(entityID, typeID, index, true)) {
			return false;
		}
		GetComponentPool(typeID)->Return(*index);
	}

	return true;
}

void ComponentManager::RemoveAllComponents(int entityID)
{
	for (auto p : entityComponentsMap[entityID]) {
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

int ComponentManager::GetObjectPoolIndex(eidType entityID, TypeId typeID, bool deleteIndex)
{
	std::vector<ObjectPoolIndex *> indices = entityComponentsMap[entityID][typeID];

	if (indices.size() == 0) {
#if defined(DEBUG) || defined(_DEBUG)
		throw "Entity did not have the component!";
#endif
		return -1;
	}

	int result = *indices.back();

	if (deleteIndex)
		indices.pop_back();

	return result;
}

bool ComponentManager::CheckObjectPoolIndex(eidType entityID, TypeId typeID, ObjectPoolIndex *index, bool deleteIndex) {
	int result = -1;

	std::vector<ObjectPoolIndex *> indices = entityComponentsMap[entityID][typeID];
	
	auto it = std::remove_if(indices.begin(), indices.end(),
		[index](const ObjectPoolIndex *i) {
		return *i == *index;
	});

	if (it == indices.end()) {
#if defined(DEBUG) || defined(_DEBUG)
		throw "Entity did not have the component!";
#endif
		return false;
	}
	else {
		if (deleteIndex)
			indices.erase(it, indices.end());

		return true;
	}
}
