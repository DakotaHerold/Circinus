#include "RigidBody.h"

using namespace DirectX;

RigidBody::RigidBody(Transform* t, const DirectX::BoundingBox* boxCollider)
{
	trans = t; 
	box = boxCollider;
}


RigidBody::~RigidBody()
{
}

void RigidBody::SetEntity(int id)
{
	this->Component::SetEntity(id);
	// TODO: Update with transform position
}

bool RigidBody::CollisionCheck(RigidBody *otherRbody)
{
	const BoundingBox &box1 = *(box);
	BoundingSphere sphere1;
	BoundingSphere::CreateFromBoundingBox(sphere1, box1);
	XMFLOAT3 rBody1Position = *(trans->GetWorldPosition());
	sphere1.Center.x += rBody1Position.x;
	sphere1.Center.y += rBody1Position.y;
	sphere1.Center.z += rBody1Position.z;

	const BoundingBox &box2 = *(otherRbody->GetBoudingBox());
	BoundingSphere sphere2;
	BoundingSphere::CreateFromBoundingBox(sphere2, box2);
	XMFLOAT3 rBody2Position = *(otherRbody->GetTransform()->GetWorldPosition());
	sphere2.Center.x += rBody2Position.x;
	sphere2.Center.y += rBody2Position.y;
	sphere2.Center.z += rBody2Position.z;

	return sphere1.Intersects(sphere2);
	return false; 
}
