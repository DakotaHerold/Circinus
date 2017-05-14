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

	void Update();
	void SetWorldVelocity(float x, float y, float z);
	void FaceTo(RigidBody* otherEntity);
	void ProjectileShootAt(RigidBody* target, float speed);
	void toEulerianAngle(DirectX::XMFLOAT4 *q, DirectX::XMFLOAT3 *euler);
	void QuatToEuler(DirectX::XMFLOAT4 *quat, DirectX::XMFLOAT3 *euler);
	//DirectX::XMFLOAT4 LookAt(DirectX::XMVECTOR target, DirectX::XMVECTOR current, DirectX::XMVECTOR eye, DirectX::XMVECTOR up);
	DirectX::XMFLOAT3 rotate(DirectX::XMFLOAT3X3* m);

private:
	Transform* trans; 
	DirectX::BoundingOrientedBox* obb;
	DirectX::XMFLOAT3 velocity; // This velocity will be a vector3 relate to world space instead of local space.
};

