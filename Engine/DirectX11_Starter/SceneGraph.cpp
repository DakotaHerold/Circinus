#include "SceneGraph.h"

#include <algorithm>

#include "Transform.h"
#include "Renderable.h"

Renderable * SceneGraph::CreateRenderable()
{
	renderables.push_back(Renderable());

	Renderable& r = renderables.back();

	return &r;
}

void SceneGraph::ClearRenderables()
{
	std::remove_if(renderables.begin(), renderables.end(), [](Renderable& r) { return r.flagDestroy; });
}
