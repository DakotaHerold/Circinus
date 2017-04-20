#include "DebugCam.h"
#include "NativeWindow.h"



void DebugCam::update(float deltaTime)
{
	handleKeyboardInput(deltaTime);

	if (isDirty)
	{
		XMVECTOR rotationQuat = XMQuaternionRotationRollPitchYaw(rotationX, rotationY, 0.0f);
		XMVECTOR rotatedVector = XMVector3Rotate(XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), rotationQuat);
		rotatedVector = XMQuaternionNormalize(rotatedVector);

		XMStoreFloat3(&direction, rotatedVector);

		XMStoreFloat3(&up, XMQuaternionNormalize(XMVector3Rotate(XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f), rotationQuat)));

		XMMATRIX vMat = XMMatrixLookToLH(XMLoadFloat3(&position), rotatedVector, XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));

		XMStoreFloat4x4(&viewMatrix, XMMatrixTranspose(vMat));

		XMMATRIX pMat = XMMatrixTranspose(XMLoadFloat4x4(&projectionMatrix));

		BoundingFrustum f;
		BoundingFrustum::CreateFromMatrix(f, pMat);
		f.Transform(frustum, XMMatrixInverse(nullptr, vMat));

		isDirty = false;
	}
}

void DebugCam::setRotationX(float rotVal)
{
	rotationX += rotVal / 1024;

	isDirty = true;
}

void DebugCam::setRotationY(float rotVal)
{
	rotationY += rotVal / 1024;

	isDirty = true;
}

void DebugCam::setProjectionMatrix(float aspectRatio)
{
	// Create the Projection matrix
	// - This should match the window's aspect ratio, and also update anytime
	//   the window resizes (which is already happening in OnResize() below)
	XMMATRIX P = XMMatrixPerspectiveFovLH(
		0.25f * 3.1415926535f,		// Field of View Angle
		aspectRatio,		// Aspect ratio
		0.1f,						// Near clip plane distance
		100.0f);					// Far clip plane distance
	XMStoreFloat4x4(&projectionMatrix, XMMatrixTranspose(P)); // Transpose for HLSL!
}

void DebugCam::moveAlongDirection(float val)
{
	position.x += val * direction.x;
	position.y += val * direction.y;
	position.z += val * direction.z;

	isDirty = true;
}

void DebugCam::moveSideways(float val)
{
	XMFLOAT3 leftSide;
	XMStoreFloat3(&leftSide, XMVector3Cross(XMLoadFloat3(&direction), XMLoadFloat3(&up)));

	position.x += val * leftSide.x;
	position.y += val * leftSide.y;
	position.z += val * leftSide.z;

	isDirty = true;
}

void DebugCam::moveVertical(float val)
{
	position.x += val * up.x;
	position.y += val * up.y;
	position.z += val * up.z;

	isDirty = true;
}

XMFLOAT3 & DebugCam::getPosition()
{
	// TODO: insert return statement here
	return position;
}

BoundingFrustum& DebugCam::getFrustum()
{
	return frustum;
}

void DebugCam::handleKeyboardInput(float moveSpeed)
{
	if (InputManager::instance().GetMovingForward())
	{
		moveAlongDirection(moveSpeed);
	}

	if (InputManager::instance().GetMovingBackward())
	{
		moveAlongDirection(-moveSpeed);
	}

	if (InputManager::instance().GetMovingLeft())
	{
		moveSideways(moveSpeed);
	}

	if (InputManager::instance().GetMovingRight())
	{
		moveSideways(-moveSpeed);
	}

	if (InputManager::instance().GetAscending())
	{
		moveVertical(moveSpeed);
	}

	if (InputManager::instance().GetDescending())
	{
		moveVertical(-moveSpeed);
	}
}

DebugCam::DebugCam()
{
	isDirty = true;

	rotationX = 0.0f;
	rotationY = 0.0f;
	position = { 0.0f, 0.0f, -5.0f };
	direction = { 0.0f, 0.0f, -1.0f };
	up = { 0.0f, 1.0f, 0.0f };

}


DebugCam::~DebugCam()
{
}

XMFLOAT4X4 & DebugCam::getViewMatrix()
{
	return viewMatrix;
}

XMFLOAT4X4 & DebugCam::getProjectionMatrix()
{
	return projectionMatrix;
}
