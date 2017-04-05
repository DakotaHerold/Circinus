#pragma once

#include "Renderable.h"

#include <list>

class RenderingSystem;

class SceneGraph
{
public:
	Renderable*		CreateRenderable();

	void			ClearRenderables();

private:
	friend class RenderingSystem;

	Renderable*		root;

	// TODO allocator for renderables
	std::list<Renderable>		renderables;
};