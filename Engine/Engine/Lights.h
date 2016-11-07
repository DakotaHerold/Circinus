#pragma once
#include <DirectXMath.h>
using namespace DirectX; 

struct DirectionalLight
{
	XMFLOAT4 AmbientColor; 
	XMFLOAT4 DiffuseColor;
	XMFLOAT3 Direction;
};

struct SpecularLight
{
	XMFLOAT4 SpecularColor; 
	XMFLOAT3 Direction;
	float SpecularStrength; 
	float LightIntensity; 
};

struct PointLight
{
	XMFLOAT4 PointLightColor; 
	XMFLOAT3 Position; 
};
