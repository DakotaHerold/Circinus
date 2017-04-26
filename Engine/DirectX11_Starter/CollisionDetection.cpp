#include "CollisionDetection.h"

using namespace DirectX;

CollisionDetection::CollisionDetection()
{
}

bool CollisionDetection::CollisionCheck(RigidBody *rBody1, RigidBody *rBody2)
{
	BoundingBox &box1 = *(rBody1->GetBoudingBox());
	BoundingSphere sphere1;
	BoundingSphere::CreateFromBoundingBox(sphere1, box1);
	XMFLOAT3 rBody1Position = *(rBody1->GetTransform()->GetWorldPosition());
	sphere1.Center.x += rBody1Position.x;
	sphere1.Center.y += rBody1Position.y;
	sphere1.Center.z += rBody1Position.z;

	BoundingBox &box2 = *(rBody2->GetBoudingBox());
	BoundingSphere sphere2;
	BoundingSphere::CreateFromBoundingBox(sphere2, box2);
	XMFLOAT3 rBody2Position = *(rBody2->GetTransform()->GetWorldPosition());
	sphere2.Center.x += rBody2Position.x;
	sphere2.Center.y += rBody2Position.y;
	sphere2.Center.z += rBody2Position.z;	

	return sphere1.Intersects(sphere2);
}


