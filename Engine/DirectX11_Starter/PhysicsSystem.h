#pragma once

#include <vector>
class RigidBody;

class PhysicsSystem
{
public:
	PhysicsSystem();
	~PhysicsSystem();

	void update(float delta, std::vector<RigidBody *> entities);
};

