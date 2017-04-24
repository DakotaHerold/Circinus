#include "TransformSystem.h"
#include "Entity.h"
#include "Transform.h"
#include "RigidBody.h"
#include "ComponentManager.h"

TransformSystem::TransformSystem()
{
}

TransformSystem::~TransformSystem()
{
}

void TransformSystem::update(float delta, ResultComponents<Transform> transforms)
{
	ComponentManager::current->root->UpdateTransform();
}
