#pragma once

#include "Renderable.h"

#include <list>

class RenderingSystem;

class SceneGraph
{
public:

	Renderable*		CreateRenderable();

	void			ClearRenderables();

	void			SetSkyBox(Renderable* skybox) { this->skybox = skybox; }

	Renderable*		GetSkyBox() { return skybox; }

private:
	friend class RenderingSystem;

	Renderable*		skybox;

	Renderable*		root;

	// TODO allocator for renderables
	std::list<Renderable>		renderables;
};