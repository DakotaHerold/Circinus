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

void TransformSystem::update(float delta, std::vector<Entity*> entities)
{
	for (Entity *entity : entities) {
		// FIXME: Get all component
		if (!entity->HasComponent<Transform>())
			return;

		Transform *transform = entity->GetComponent<Transform>();

		if (transform->IsDirty()) {
			RigidBody *body = entity->GetComponent<RigidBody>();

			if (body) {
				body->SetPosition(*transform->GetPosition());
			}
		}

		//if (transform) {
		//	XMMATRIX m = XMMatrixMultiply(
		//		XMMatrixMultiply(
		//			XMMatrixScaling(transform->GetScale()->x, transform->GetScale()->y, transform->GetScale()->z),
		//			XMMatrixRotationRollPitchYaw(transform->GetRotation()->x, transform->GetRotation()->y, transform->GetRotation()->z)
		//		),
		//		XMMatrixTranslation(transform->GetPosition()->x, transform->GetPosition()->y, transform->GetPosition()->z)
		//	);

		//	XMStoreFloat4x4(
		//		transform->GetLocalMatrix(),
		//		XMMatrixTranspose(m)
		//	);

		//	if (nullptr != transform->GetParent())
		//	{
		//		XMMATRIX w = XMMatrixMultiply(
		//			m,
		//			XMLoadFloat4x4(transform->GetParent()->GetWorldMatrix())
		//		);

		//		XMStoreFloat4x4(
		//			transform->GetWorldMatrix(),
		//			XMMatrixTranspose(w)
		//		);
		//	}
		//	else
		//	{
		//		XMStoreFloat4x4(
		//			transform->GetWorldMatrix(),
		//			XMMatrixTranspose(m)
		//		);
		//	}

		//	//dirty = false;
		//	//modified = true;
		//}
	}
}
