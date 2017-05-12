#pragma once
#include "Component.h"
#include "Transform.h"
#include <DirectXMath.h>
#include <DirectXCollision.h>

class RigidBody :
	public Component
{
public:
	RigidBody(Transform* t, const DirectX::BoundingBox* boxCollider);
	~RigidBody();

	void SetEntity(int id) override;

	#pragma region Getters and Setters
	Transform* GetTransform() { return trans; }
	DirectX::BoundingOrientedBox* GetBoudingBox() { return obb; }
	#pragma endregion 

	bool SphereCollisionCheck(RigidBody *otherRbody);
	bool BoxCollisionCheck(RigidBody *otherRbody);

	void Orbit(RigidBody *otherBody, float radius, float speed, float deltaTime);

private:
	Transform* trans; 
	DirectX::BoundingOrientedBox* obb; 
	DirectX::XMFLOAT3 velocity;
	float mass;
};