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

void RigidBody::SetEntity(Entity * entity)
{
	this->Component::SetEntity(entity);
	// TODO: Update with transform position
}


