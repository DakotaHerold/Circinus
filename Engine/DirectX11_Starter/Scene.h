#pragma once

#if defined(_DEBUG)
#include "DebugCam.h"
typedef DebugCam Camera;
#else
#include "Camera.h"
#endif

#include "SceneGraph.h"
#include "ComponentManager.h"
class Entity;
class Material;

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

private:
	Camera			cam;
	SceneGraph		sceneGraph;

	Material*		mat;
	Entity*			enti;

};
