#pragma once
#include "Component.h"
#include<vector>
#include <utility>
#include "Entity.h"
class ComponentManager
{
public:
	ComponentManager();
	~ComponentManager();
	static void NewComManager();
	void AddComponent(int entityID, Component* component, TypeId componentTypeId);
	bool RemoveComponent(int entityID, TypeId componentTypeId);
	Component * GetComponent(int entityID, TypeId componentTypeId) const;
	bool HasComponent(int entityID, TypeId componentTypeId) const;
	static ComponentManager* GetCurrent();
private:
	std::vector<std::pair<TypeId, Component*>> allComponets;
	static ComponentManager* current;
	
	
};

