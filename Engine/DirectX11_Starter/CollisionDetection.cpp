#include "CollisionDetection.h"

CollisionDetection::CollisionDetection()
{
}

bool CollisionDetection::SphereCollision(BoundingSphere & sphere1, BoundingSphere & sphere2)
{
	return sphere1.Intersects(sphere2);
}

bool CollisionDetection::BoxCollsion(BoundingBox & box1, BoundingBox & box2)
{
	return box1.Intersects(box2);
}


