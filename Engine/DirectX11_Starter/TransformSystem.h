#pragma once

#include <vector>
#include "Transform.h"

class TransformSystem
{
public:
	TransformSystem();
	~TransformSystem();

	void update(float delta, std::vector<Transform*> &transforms);
};

