#pragma once
#include "Component.h"
#include "Engine.h"
#include "Scene.h"
#include "Camera.h"

class Camera;

class CameraComponent :
	public Component
{
private:
	Camera cam;
public:
	CameraComponent();
	~CameraComponent();

	void Update();
	void UpdateCameraValues(XMFLOAT3& pos, XMFLOAT3& rot);
	Camera* GetCamera() { return &cam; }

	void Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) override;
};

