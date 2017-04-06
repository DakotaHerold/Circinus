#include "RigidBody.h"



RigidBody::RigidBody()
{
}


RigidBody::~RigidBody()
{
}



void RigidBody::SetPosition(DirectX::XMFLOAT3 position)
{
	_position = position;
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


