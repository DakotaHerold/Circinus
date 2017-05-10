#pragma once

#include <vector>
#include "Transform.h"
#include "ComponentManager.h"

class TransformSystem
{
public:
	TransformSystem();
	~TransformSystem();

	void update(float delta, ResultComponents<Transform> transforms);
};

