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

void ComponentManager::RemoveAllComponents(int entityID)
{
	for (auto p : entityComponentsMap[entityID]) {
		ObjectPoolBase *pool = ComponentPoolsMap[p.first];
		
		for (ObjectPoolIndex *i : p.second) {
			std::cout << *i << std::endl;


			//pool->Return(*i);
		}
	}
}