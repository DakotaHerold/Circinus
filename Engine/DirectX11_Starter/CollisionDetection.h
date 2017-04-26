#pragma once
#include <DirectXCollision.h>

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

	bool SphereCollision(BoundingSphere &sphere1, BoundingSphere &sphere2);
	bool BoxCollsion(BoundingBox &box1, BoundingBox &box2);

private:
	// Constructor 
	CollisionDetection();
};

