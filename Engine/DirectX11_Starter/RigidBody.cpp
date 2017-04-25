#include "RigidBody.h"



RigidBody::RigidBody()
{
}


RigidBody::~RigidBody()
{
	delete trans; 
}

void RigidBody::SetPosition(DirectX::XMFLOAT3 position)
{
	trans->SetLocalPosition(position.x, position.y, position.z);
}

void RigidBody::SetPosition(float x, float y, float z)
{
	trans->SetLocalPosition(x, y, z);
}

bool RigidBody::IsDirty()
{
	return dirty;
}

void RigidBody::SetEntity(int id)
{
	this->Component::SetEntity(id);
	// TODO: Update with transform position
}


