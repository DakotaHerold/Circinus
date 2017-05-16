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
	RigidBody() :
		velocity(0, 0, 0) {};
	void Init(Transform* t, const DirectX::BoundingBox* boxCollider) {
		trans = t;
		obb = new BoundingOrientedBox();
		DirectX::BoundingOrientedBox::CreateFromBoundingBox(*obb, *boxCollider);
	}
	~RigidBody();

	void SetEntity(EntityID eid) override;

	#pragma region Getters and Setters
	Transform* GetTransform() { return trans; }
	DirectX::BoundingOrientedBox* GetBoudingBox() { return obb; }
	#pragma endregion 

	bool SphereCollisionCheck(RigidBody *otherRbody);
	bool BoxCollisionCheck(RigidBody *otherRbody);

	void Update();
	void SetWorldVelocity(float x, float y, float z);
	void FaceTo(RigidBody* otherEntity);
	void ProjectileHomingAt(RigidBody* target, float speed);
	void ProjectileShootAt(RigidBody* target, float speed);
	DirectX::XMFLOAT3 RotateToNewEuler(DirectX::XMFLOAT3X3* m);
	DirectX::XMFLOAT3 velocity; // This velocity will be a vector3 relate to world space instead of local space.

	void Orbit(RigidBody *otherBody, float radius, float speed, float deltaTime);
	void Seek(RigidBody *otherBody, float speed);

	void	Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) {
		writer.StartObject();
		writer.String("name");
		writer.String("RigidBody");
		writer.EndObject();
	}
	void	Load(rapidjson::Value v) {}

private:
	Transform* trans; 
	DirectX::BoundingOrientedBox* obb;
};

