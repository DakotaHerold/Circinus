#include "RigidBody.h"

using namespace DirectX;

RigidBody::RigidBody(Transform* t, const DirectX::BoundingBox* boxCollider)
{
	trans = t; 
	obb = new BoundingOrientedBox(); 
	DirectX::BoundingOrientedBox::CreateFromBoundingBox(*obb, *boxCollider);
}


RigidBody::~RigidBody()
{
	delete obb; 
}

void RigidBody::SetEntity(int id)
{
	this->Component::SetEntity(id);
	// TODO: Update with transform position
}
//
//bool RigidBody::SphereCollisionCheck(RigidBody *otherRbody)
//{
//	const BoundingBox &box1 = *(obb);
//	BoundingSphere sphere1;
//	BoundingSphere::CreateFromBoundingBox(sphere1, box1);
//	XMFLOAT3 rBody1Position = *(trans->GetLocalPosition());
//	sphere1.Center.x += rBody1Position.x;
//	sphere1.Center.y += rBody1Position.y;
//	sphere1.Center.z += rBody1Position.z;
//
//	const BoundingBox &box2 = *(otherRbody->obb);
//	BoundingSphere sphere2;
//	BoundingSphere::CreateFromBoundingBox(sphere2, box2);
//	XMFLOAT3 rBody2Position = *(otherRbody->GetTransform()->GetLocalPosition());
//	sphere2.Center.x += rBody2Position.x;
//	sphere2.Center.y += rBody2Position.y;
//	sphere2.Center.z += rBody2Position.z;
//
//	return sphere1.Intersects(sphere2);
//}

bool RigidBody::BoxCollisionCheck(RigidBody * otherRbody)
{
	XMFLOAT3* pos1 = trans->GetLocalPosition(); 
	obb->Center.x = pos1->x;
	obb->Center.x = pos1->y; 
	obb->Center.x = pos1->z;

	XMFLOAT3* pos2 = otherRbody->trans->GetLocalPosition(); 
	otherRbody->obb->Center.x = pos2->x; 
	otherRbody->obb->Center.y = pos2->y;
	otherRbody->obb->Center.z = pos2->z;

	//BoundRenderer::instance()->Draw(*obb);
	//BoundRenderer::instance()->Draw(*otherRbody->obb);

	return obb->Intersects(*(otherRbody->obb));
}
