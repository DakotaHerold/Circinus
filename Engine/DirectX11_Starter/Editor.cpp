#include "Editor.h"

#ifdef EDITOR_BUILD

Editor * Editor::instance()
{
	static Editor* _instance = new Editor();
	return _instance;
}

void Editor::Update(float deltaTime, float totalTime)
{
}

#endif