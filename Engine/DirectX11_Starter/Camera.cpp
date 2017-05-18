#include "Camera.h"
#include "NativeWindow.h"



void Camera::update(float deltaTime)
{
	if (isDirty)
	{
		XMVECTOR rotatedVector = XMLoadFloat3(&direction);

		XMMATRIX vMat = XMMatrixLookToLH(XMLoadFloat3(&position), rotatedVector, XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));

		XMStoreFloat4x4(&viewMatrix, XMMatrixTranspose(vMat));

		XMMATRIX pMat = XMMatrixTranspose(XMLoadFloat4x4(&projectionMatrix));

		BoundingFrustum f;
		BoundingFrustum::CreateFromMatrix(f, pMat);
		f.Transform(frustum, XMMatrixInverse(nullptr, vMat));

		isDirty = false;
	}
}

void Camera::setRotationX(float rotVal)
{
	rotationX += rotVal / 1024;

	XMVECTOR rotationQuat = XMQuaternionRotationRollPitchYaw(rotationX, rotationY, 0.0f);
	XMVECTOR rotatedVector = XMVector3Rotate(XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), rotationQuat);
	rotatedVector = XMQuaternionNormalize(rotatedVector);

	XMStoreFloat3(&direction, rotatedVector);

	XMStoreFloat3(&up, XMQuaternionNormalize(XMVector3Rotate(XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f), rotationQuat)));

	isDirty = true;
}

void Camera::setRotationY(float rotVal)
{
	rotationY += rotVal / 1024;

	XMVECTOR rotationQuat = XMQuaternionRotationRollPitchYaw(rotationX, rotationY, 0.0f);
	XMVECTOR rotatedVector = XMVector3Rotate(XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), rotationQuat);
	rotatedVector = XMQuaternionNormalize(rotatedVector);

	XMStoreFloat3(&direction, rotatedVector);

	XMStoreFloat3(&up, XMQuaternionNormalize(XMVector3Rotate(XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f), rotationQuat)));

	isDirty = true;
}

void Camera::setViewMatrix(XMFLOAT3 & dir)
{
	XMMATRIX vMat = XMMatrixLookToLH(XMLoadFloat3(&position), XMLoadFloat3(&dir), XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));

	XMStoreFloat4x4(&viewMatrix, XMMatrixTranspose(vMat));
}

void Camera::setProjectionMatrix(float aspectRatio)
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

void Camera::setPosition(XMFLOAT3 & pos)
{
	position = pos;

	isDirty = true;
}

void Camera::setDirection(XMFLOAT3 & dir)
{
	direction = dir;

	isDirty = true;
}

void Camera::moveAlongDirection(float val)
{
	position.x += val * direction.x;
	position.y += val * direction.y;
	position.z += val * direction.z;

	isDirty = true;
}

void Camera::moveSideways(float val)
{
	XMFLOAT3 leftSide;
	XMStoreFloat3(&leftSide, XMVector3Cross(XMLoadFloat3(&direction), XMLoadFloat3(&up)));

	position.x += val * leftSide.x;
	position.y += val * leftSide.y;
	position.z += val * leftSide.z;

	isDirty = true;
}

void Camera::moveVertical(float val)
{
	position.x += val * up.x;
	position.y += val * up.y;
	position.z += val * up.z;

	isDirty = true;
}

XMFLOAT3 & Camera::getPosition()
{
	// TODO: insert return statement here
	return position;
}

BoundingFrustum& Camera::getFrustum()
{
	return frustum;
}

void Camera::handleKeyboardInput(float moveSpeed)
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

Camera::Camera()
{
	isDirty = true;

	rotationX = 0.0f;
	rotationY = 0.0f;
	position = { 0.0f, 0.0f, 0.0f };
	direction = { 0.0f, 0.0f, 1.0f };
	up = { 0.0f, 1.0f, 0.0f };

	setProjectionMatrix(800.0f / 600.0f);
}


Camera::~Camera()
{
}

XMFLOAT4X4 & Camera::getViewMatrix()
{
	return viewMatrix;
}

XMFLOAT4X4 & Camera::getProjectionMatrix()
{
	return projectionMatrix;
}
