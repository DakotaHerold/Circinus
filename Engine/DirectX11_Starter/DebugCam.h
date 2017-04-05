#pragma once
#include <DirectXMath.h>
#include <math.h>
#include "DirectXGameCore.h"
#include "Vertex.h"
#include "InputManager.h"

// For the DirectX Math library
using namespace DirectX;

class DebugCam
{
private:
	XMFLOAT4X4 viewMatrix;
	XMFLOAT4X4 projectionMatrix;
	XMFLOAT3 position;
	XMFLOAT3 direction;
	XMFLOAT3 up;
	float rotationX;
	float rotationY;

	void handleKeyboardInput(float moveSpeed);

	// Bool to check if values have changed since last frame
	bool isDirty;

public:
	DebugCam();
	~DebugCam();
	XMFLOAT4X4& getViewMatrix();
	XMFLOAT4X4& getProjectionMatrix();
	void update(float deltaTime);
	void setRotationX(float rotVal);
	void setRotationY(float rotVal);
	void setProjectionMatrix(float aspectRatio);
	void moveAlongDirection(float val);
	void moveSideways(float val);
	void moveVertical(float val);
	XMFLOAT3& getPosition();
};
