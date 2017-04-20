#pragma once
#include "Component.h"
#include<vector>
#include <utility>
#include "Entity.h"
#include "Transform.h"
#include "RigidBody.h"
#include "Renderable.h"

class Scene;
class ComponentManager
{
public:
	ComponentManager();
	~ComponentManager();
	void AddComponent(int entityID, Component* component, TypeId componentTypeId);
	bool RemoveComponent(int entityID, TypeId componentTypeId);
	Component * GetComponent(int entityID, TypeId componentTypeId) const;
	bool HasComponent(int entityID, TypeId componentTypeId) const;

	
	std::vector<Transform*> transfromComponents;
	std::vector<RigidBody*> rigidBodyComponents;
	std::vector<Renderable*> renderables;
	void Release();

	Transform* root;
	static ComponentManager* current;	
private:
	Scene* curScene;
	
};
