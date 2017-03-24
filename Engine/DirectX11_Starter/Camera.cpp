#include "Camera.h"



Camera::Camera()
{
	//initialize attributes
	position = XMFLOAT3(0.0f, 0.0f, -5.0f); 
	direction = XMFLOAT3(0.0f, 0.0f, 1.0f); 
	left = XMFLOAT3(0.0f, 0.0f, 0.0f); 
	up = XMFLOAT3(0.0f, 1.0f, 0.0f); 
	forward = XMFLOAT3(0.0f, 0.0f, 1.0f);
	pitch = 0.0f; 
	yaw = 0.0f; 
	XMMATRIX rotMat = XMLoadFloat4x4(&rotationMatrix); 
	rotMat = XMMatrixIdentity(); 
	XMStoreFloat4x4(&rotationMatrix, rotMat); 

	//initialize viewMatrix 
	XMVECTOR pos = XMVectorSet(0, 0, -5, 0);
	XMVECTOR dir = XMVectorSet(0, 0, 1, 0);
	XMVECTOR up = XMVectorSet(0, 1, 0, 0);
	XMMATRIX V = XMMatrixLookToLH(
		pos,     // The position of the "camera"
		dir,     // Direction the camera is looking
		up);     // "Up" direction in 3D space (prevents roll)
	XMStoreFloat4x4(&viewMatrix, XMMatrixTranspose(V)); // Transpose for HLSL!

	// Initialize sensitivity buffer
	sensitivityBuffer = 0.0005f;
	cameraMoveSpeed = 0.01f;
}


Camera::~Camera()
{
}

void Camera::onResize(float aspectRatio)
{
	// Update our projection matrix since the window size changed
	XMMATRIX P = XMMatrixPerspectiveFovLH(
		0.25f * 3.1415926535f,	// Field of View Angle
		aspectRatio,		  	// Aspect ratio
		0.1f,				  	// Near clip plane distance
		100.0f);			  	// Far clip plane distance
	XMStoreFloat4x4(&projectionMatrix, XMMatrixTranspose(P)); // Transpose for HLSL!
}

void Camera::update(float deltaTime)
{

	//Update view matrix based on current camera's position
	//Load variables in for operations 
	XMVECTOR camForward = XMLoadFloat3(&forward); 
	XMVECTOR camUp = XMLoadFloat3(&up);
	XMVECTOR camLeft = XMVector3Cross(camForward, camUp);
	XMStoreFloat3(&left, camLeft); 
	XMVECTOR camPos = XMLoadFloat3(&position);
	XMMATRIX viewMat = XMLoadFloat4x4(&viewMatrix);
	viewMat = XMMatrixLookToLH(camPos, camForward, camUp); 
	XMStoreFloat4x4(&viewMatrix, XMMatrixTranspose(viewMat)); 


}

void Camera::cameraInput(float deltaTime)
{
	if (GetAsyncKeyState('W') & 0x8000)
	{
		moveForward(cameraMoveSpeed);
	}
	if (GetAsyncKeyState('A') & 0x8000)
	{
		strafe(-cameraMoveSpeed);
	}
	if (GetAsyncKeyState('S') & 0x8000)
	{
		moveForward(-cameraMoveSpeed);
	}
	if (GetAsyncKeyState('D') & 0x8000)
	{
		strafe(cameraMoveSpeed);
	}
	if (GetAsyncKeyState('X') & 0x8000)
	{
		moveVertically(-cameraMoveSpeed);
	}
	if (GetAsyncKeyState(VK_SPACE) & 0x8000)
	{
		moveVertically(cameraMoveSpeed);
	}


}

void Camera::rotate(float amt)
{
	XMVECTOR fwd = XMLoadFloat3(&forward);
	XMVECTOR camPos = XMLoadFloat3(&position);
	XMVECTOR rotation = XMQuaternionRotationRollPitchYaw(0, amt, 0);  
	fwd = XMVector3Rotate(fwd, rotation);
	XMStoreFloat3(&forward, fwd);
}

void Camera::moveForward(float displacement)
{
	XMVECTOR camPos = XMLoadFloat3(&position);
	XMVECTOR fwd = XMLoadFloat3(&forward);
	camPos += fwd * displacement;
	XMStoreFloat3(&position, camPos);
}

void Camera::moveVertically(float displacement)
{
	XMVECTOR camPos = XMLoadFloat3(&position);
	XMVECTOR tempUp = XMLoadFloat3(&up);
	camPos += tempUp * displacement;
	XMStoreFloat3(&position, camPos);
}

void Camera::strafe(float displacement)
{
	XMVECTOR camPos = XMLoadFloat3(&position);
	XMVECTOR right = -XMLoadFloat3(&left);
	camPos += right * displacement;
	XMStoreFloat3(&left, right);
	XMStoreFloat3(&position, camPos);
}

void Camera::turnWithMouse(float dx, float dy)
{
	pitch = dy * sensitivityBuffer;
	yaw = dx * sensitivityBuffer;
	pitch = restrictAngle(pitch); 
	yaw = restrictAngle(yaw); 
	XMVECTOR fwd = XMLoadFloat3(&forward); 
	XMVECTOR camPos = XMLoadFloat3(&position); 
	XMMATRIX rotMat = XMMatrixRotationRollPitchYaw(pitch, yaw, 0.0f); 
	fwd = XMVector3TransformCoord(fwd, rotMat); 
	fwd = XMVector3Normalize(fwd); 
	XMStoreFloat3(&forward, fwd); 
}

void Camera::turnWithController(float dx, float dy)
{
	pitch = dy * 0.0005f;
	yaw = dx * 0.0005f;
	pitch = restrictAngle(pitch);
	yaw = restrictAngle(yaw);
	XMVECTOR fwd = XMLoadFloat3(&forward);
	XMVECTOR camPos = XMLoadFloat3(&position);
	XMMATRIX rotMat = XMMatrixRotationRollPitchYaw(pitch, yaw, 0.0f);
	fwd = XMVector3TransformCoord(fwd, rotMat);
	fwd = XMVector3Normalize(fwd);
	XMStoreFloat3(&forward, fwd);
}

float Camera::restrictAngle(float angle)
{
	float pie = (float)M_PI; 
	while (angle > 2 * pie)
	{
		angle -= 2 * pie; 
	}
	while (angle < 0)
	{
		angle += 2 * pie; 
	}
	return angle;
}

XMFLOAT4X4 Camera::getProjectionMatrix()
{
	return projectionMatrix; 
}

XMFLOAT4X4 Camera::getViewMatrix()
{
	return viewMatrix; 
}

XMFLOAT3 Camera::getUp()
{
	return up; 
}

XMFLOAT3 Camera::getForward()
{
	return forward; 
}

XMFLOAT3 Camera::getLeft()
{
	return left; 
}

XMFLOAT3 Camera::getDirection()
{
	return direction;
}

XMFLOAT3 Camera::getPosition()
{
	return position;
}

void Camera::setProjectionMatrix(XMFLOAT4X4 newMat)
{
	projectionMatrix = newMat; 
}

void Camera::setViewMatrix(XMFLOAT4X4 newMat)
{
	viewMatrix = newMat; 
}

void Camera::setPitch(float newPitch)
{
	pitch = newPitch;
}

void Camera::setYaw(float newYaw)
{
	yaw = newYaw; 
}

