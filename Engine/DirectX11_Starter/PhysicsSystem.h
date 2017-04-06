#pragma once

#include <vector>
class Entity;

class PhysicsSystem
{
public:
	PhysicsSystem();
	~PhysicsSystem();

	void update(float delta, std::vector<Entity *> entities);
};

