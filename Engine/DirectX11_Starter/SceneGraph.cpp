#include "SceneGraph.h"

#include <algorithm>

#include "Transform.h"
#include "Renderable.h"

Renderable * SceneGraph::CreateRenderable()
{
	renderables.push_back(Renderable());

	Renderable& r = renderables[renderables.size() - 1];

	// TODO : transform from allocator
	r.transform = new Transform();

	return &r;
}

void SceneGraph::ClearRenderables()
{
	std::remove_if(renderables.begin(), renderables.end(), [](Renderable& r) { return r.flagDestroy; });
}
