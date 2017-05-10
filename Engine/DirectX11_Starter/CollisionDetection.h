#pragma once

#include <DirectXCollision.h>
#include "RigidBody.h"

using namespace DirectX;

class CollisionDetection
{
public:
	// Singleton reference
	static CollisionDetection& instance()
	{
		static CollisionDetection cd_instance;
		return cd_instance;
	}

	// Singleton destructors
	CollisionDetection(CollisionDetection const&) = delete;

	bool CollisionCheck(RigidBody *rBody1, RigidBody *rBody2);

private:
	// Constructor 
	CollisionDetection();
};

