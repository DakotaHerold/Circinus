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
	int i = getObjectIndex(entityID, typeID, index, true);

	if (i == -1) {
		return false;
	}
	else {
		GetComponentPool(typeID)->Return(i);
		return true;
	}
}

void ComponentManager::RemoveAllComponents(int entityID)
{
	for (auto p : entityComponentsMap[entityID]) {
		for (ObjectPoolIndex *i : p.second) {
			GetComponentPool(p.first)->Return(*i);
		}
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

int ComponentManager::getObjectIndex(eidType entityID, TypeId typeID, ObjectPoolIndex *index, bool deleteIndex)
{
	std::vector<ObjectPoolIndex *> indices = entityComponentsMap[entityID][typeID];

	int result = -1;

	if (index == nullptr) {
		result = *indices.back();

		if(deleteIndex)
			indices.pop_back();
	}
	else {
		auto it = std::remove_if(indices.begin(), indices.end(), 
			[index](const ObjectPoolIndex *i) {
			return *i == *index;
		});

		if (it != indices.end()) {
			result = *index;

			if(deleteIndex)
				indices.erase(it, indices.end());
		}
	}

	if (result = -1) {
#if defined(DEBUG) || defined(_DEBUG)
		throw "No that component";
#endif
	}

	return result;
}
