#include "PhysicsSystem.h"
#include "RigidBody.h"
#include "Entity.h"
#include "Transform.h"

PhysicsSystem::PhysicsSystem()
{
}

PhysicsSystem::~PhysicsSystem()
{
}

void PhysicsSystem::update(float delta, std::vector<RigidBody*> entities)
{
	//for (Entity *entity : entities) {
	//	RigidBody *body = entity->GetComponent<RigidBody>();

	//	if (!body) {
	//		continue;
	//	}

	//	if (body->IsDirty()) {
	//		entity->GetComponent<Transform>()->SetPosition(body->GetPosition()->x, body->GetPosition()->y, body->GetPosition()->z);

	//		// TODO: Rotation, Scale(?)
	//	}
	//}
}

