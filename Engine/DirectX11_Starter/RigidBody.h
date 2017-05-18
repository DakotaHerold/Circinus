#pragma once
#include "Component.h"
#include "Transform.h"
#include "Scene.h"
#include "Engine.h"
#include <DirectXMath.h>
#include <DirectXCollision.h>
#include "time.h"
#include "ParticleEmitter.h"

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

	void Update(float deltatime);
	void SetWorldVelocity(float x, float y, float z);
	void FaceTo(RigidBody* otherEntity);
	void ProjectileHomingAt(RigidBody* target, float speed);
	void ProjectileShootAt(RigidBody* target, float speed);
	DirectX::XMFLOAT3 RotateToNewEuler(DirectX::XMFLOAT3X3* m);
	DirectX::XMFLOAT3 velocity; // This velocity will be a vector3 relate to world space instead of local space.
	void ProjectileSwarmingAt(string name, float speed, int minTurnTime, int maxTurnTime, int maxOffAngle); //minTurnTime and maxTurnTime should be represented as ms (1000ms = 1s)
	void ShootLaser(string parentName, float speed);

	Scene* curScene;

private:
	Transform* trans; 
	DirectX::BoundingOrientedBox* obb;
	float RigidDeltaTime;

	//Better swarm homing
	bool IsSlerping;
	void ProjectileSlerpAngle(DirectX::XMVECTOR* angle, DirectX::XMVECTOR* targetAngle, float t);
	float slerpT;
	float timeSinceLastLoop;
	int slerpTime;
	DirectX::XMVECTOR currentQuat;
	DirectX::XMVECTOR newQuat;
	//Better swarm homing

	bool laser;
	bool shoot;
	bool swarm;
};

