#include "TransformSystem.h"
#include "Entity.h"
#include "Transform.h"
#include "RigidBody.h"

TransformSystem::TransformSystem()
{
}

TransformSystem::~TransformSystem()
{
}

void TransformSystem::update(float delta, std::vector<Transform*> &transforms)
{
	ComponentManager::current->root->UpdateTransform();
}
