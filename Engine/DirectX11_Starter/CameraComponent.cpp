#include "CameraComponent.h"

CameraComponent::CameraComponent()
{
	//Engine::instance()->GetCurScene()->GetCamera()->setAttachedEntity(ent);
}


CameraComponent::~CameraComponent()
{
}

void CameraComponent::Update()
{
	Transform* playerT = GetEntity()->GetComponent<Transform>();

	UpdateCameraValues(*playerT->GetWorldPosition(), *playerT->GetWorldRotation());
}

void CameraComponent::UpdateCameraValues(XMFLOAT3 & pos, XMFLOAT3 & rot)
{
	//Update rotation first because it updates the direction of the camera
	cam.setRotationX(rot.x);
	cam.setRotationY(rot.y);

	XMFLOAT3 position;
	XMStoreFloat3(&position, XMLoadFloat3(&pos) - XMLoadFloat3(&cam.getDirection()) * 10);
	cam.setPosition(pos);

	cam.update(NULL);
}
