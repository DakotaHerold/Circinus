#pragma once

#include <DirectXMath.h>

class Entity;

class HandleGizmo
{
public:

	void Update(
		Entity* entity, 
		float deltaTime, 
		float totalTime, 
		DirectX::FXMVECTOR rayStart, 
		DirectX::FXMVECTOR rayDir);

private:

};