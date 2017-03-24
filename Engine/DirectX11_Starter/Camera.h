#pragma once

#include <DirectXMath.h>
#include "Vertex.h"
#include "DirectXGameCore.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <d3d11.h>
#include <iostream>
#include <vector>

#define DEGTORAD(degree) (((float)M_PI / 180.0f) * (degree)) // converts from degrees to radians
#define RADTODEG(radian) ((180.0f / (float)M_PI) * (radian)) // converts from radians to degrees

using namespace DirectX;
class Camera
{
public:
	Camera();
	~Camera();

	//Class specific functions
	void onResize(float aspectRatio);
	void update(float deltaTime); 
	void cameraInput(float deltaTime);
	void rotate(float amt); 
	void moveForward(float displacement); 
	void moveVertically(float displacement); 
	void strafe(float displacement); 
	void turnWithMouse(float dx, float dy);
	void turnWithController(float dx, float dy);
	float restrictAngle(float angle); 

	// Getters and Setters 
	XMFLOAT4X4 getProjectionMatrix(); 
	XMFLOAT4X4 getViewMatrix(); 
	XMFLOAT3 getUp(); 
	XMFLOAT3 getForward(); 
	XMFLOAT3 getLeft(); 
	XMFLOAT3 getDirection(); 
	XMFLOAT3 getPosition(); 
	void setProjectionMatrix(XMFLOAT4X4 newMat); 
	void setViewMatrix(XMFLOAT4X4 newMat); 
	void setPitch(float newPitch); 
	void setYaw(float newYaw); 


private:
	XMFLOAT4X4 projectionMatrix; 
	XMFLOAT4X4 viewMatrix; 
	XMFLOAT4X4 rotationMatrix;  
	XMFLOAT3 position;
	XMFLOAT3 forward;
	XMFLOAT3 left; 
	XMFLOAT3 up; 
	XMFLOAT3 direction; 
	float pitch; 
	float yaw; 
	float cameraMoveSpeed; 
	float sensitivityBuffer; 
};

