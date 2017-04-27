#pragma once
#include "Component.h"
#include "Transform.h"
#include <DirectXMath.h>
#include <DirectXCollision.h>

class RigidBody :
	public Component
{
public:
	RigidBody(Transform* t, DirectX::BoundingBox* boxCollider);
	~RigidBody();

	void SetEntity(int id) override;

	#pragma region Getters and Setters
	Transform* GetTransform() { return trans; }
	DirectX::BoundingBox* GetBoudingBox() { return box; }
	#pragma endregion 

	bool CollisionCheck(RigidBody *otherRbody);

private:
	Transform* trans; 
	DirectX::BoundingBox* box; 
};

