#include "RigidBody.h"



RigidBody::RigidBody(Transform* t, DirectX::BoundingBox* boxCollider)
{
	trans = t; 
	box = boxCollider;
}


RigidBody::~RigidBody()
{
	delete trans; 
}

void RigidBody::SetEntity(int id)
{
	this->Component::SetEntity(id);
	// TODO: Update with transform position
}


