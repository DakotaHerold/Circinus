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
	void UpdateCameraValues(XMFLOAT3& pos, XMFLOAT3& dir);			//Update the camera position and direction with the parameters provided
public:
	CameraComponent();
	~CameraComponent();

	// Updates the position and direction of the camera with respect to the entity it is attached to
	void Update();
	Camera* GetCamera() { return &cam; }

	// Serialization code for Camera
	void Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) override;
};

