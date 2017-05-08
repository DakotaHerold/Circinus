#pragma once

#if defined(_DEBUG)
#include "DebugCam.h"
typedef DebugCam Camera;
#else
#include "Camera.h"
#endif
#include <vector>
#include "SceneGraph.h"
#include "OctTree.h"
#include "ComponentManager.h"
class Entity;
class Material;

typedef vector<Entity*> EntityVector;

// 
//		NOTE:
//		This is just a hardcoded stub class, for debug
//		Replace this if it's your working area!
//
class Scene
{
public:
	void Enter();

	void Tick(float deltaTime, float totalTime);

	void Exit();

	Camera* GetCamera() { return &cam; }
	SceneGraph* GetSceneGraph() { return &sceneGraph; }
	ComponentManager* componentManager;
	EntityVector GetAllEntities();
	void AddEntity(Entity* entity);
	Entity* GetEntityByName(string name);
	Entity* GetEntityByID(int id);
private:
	Camera			cam;
	SceneGraph		sceneGraph;

	Material*		mat;
	vector<Entity*> entities;
	Entity*			lights;
	DirectX::BoundingOrientedBox box; 
	Octree* oct; 
};
