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

	DirectX::XMMATRIX worldMat = DirectX::XMMatrixTranspose(DirectX::XMLoadFloat4x4(playerT->GetWorldMatrix()));
	DirectX::XMVECTOR rotQuat = DirectX::XMQuaternionRotationMatrix(worldMat);
	DirectX::XMFLOAT3 dir;
	DirectX::XMStoreFloat3(&dir, DirectX::XMVector3Rotate(DirectX::XMVectorSet(0, 0, 1, 0), rotQuat));

	UpdateCameraValues(*playerT->GetWorldPosition(), dir);
}

void CameraComponent::UpdateCameraValues(XMFLOAT3& pos, XMFLOAT3 & dir)
{
	cam.setDirection(dir);

	XMFLOAT3 position;
	XMStoreFloat3(&position, XMLoadFloat3(&pos) - XMLoadFloat3(&(cam.getDirection())) * 10);
	cam.setPosition(position);

	cam.update(NULL);
}

void CameraComponent::Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer)
{
	writer.StartObject();
	writer.String("name");
	writer.String("Camera");
	writer.EndObject();
}
