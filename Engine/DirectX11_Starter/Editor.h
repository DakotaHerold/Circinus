#pragma once

#include "Config.h"

#ifdef EDITOR_BUILD

#include "Camera.h"
#include "HandleGizmo.h"

class Entity;

class Editor
{
private:
	static Editor* _instance;

public:
	static Editor* instance() { return _instance; }

public:

	void Init();

	void Update(float deltaTime, float totalTime);

	void CleanUp();

	Camera* GetEditorCamera() { return &cam; }

private:
	Camera		cam;
	HandleGizmo	handle;
	Entity*		selectedEntity;
};

#endif