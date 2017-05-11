#pragma once

#include "Config.h"

#ifdef EDITOR_BUILD

#include "DebugCam.h"

class Editor
{
public:
	static Editor* instance();

public:

	void Update(float deltaTime, float totalTime);

private:
	DebugCam		cam;
};

#endif