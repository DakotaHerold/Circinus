#include "HandleGizmo.h"

#include "InputManager.h"
#include "Entity.h"
#include "Transform.h"

void HandleGizmo::Update(
	Entity* entity, 
	float deltaTime, 
	float totalTime, 
	DirectX::FXMVECTOR rayStart,
	DirectX::FXMVECTOR rayDir)
{
	InputManager& input = InputManager::instance();

	auto t = entity->GetComponent<Transform>();


}
