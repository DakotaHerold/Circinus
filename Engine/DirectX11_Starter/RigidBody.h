#pragma once
#include "Component.h"
#include "Transform.h"
#include <DirectXMath.h>

class RigidBody :
	public Component
{
public:
	RigidBody();
	~RigidBody();

	void SetEntity(int id) override;

	#pragma region Getters and Setters
	void					SetPosition(float x, float y, float z);
	void					SetPosition(DirectX::XMFLOAT3 position);
	void					SetRotationEuler(float x, float y, float z);

	DirectX::XMFLOAT3*		GetLocalPosition() { return &trans->GetLocalPosition; }
	DirectX::XMFLOAT3*		GetLocalRotation() { return &trans->GetLocalRotation; }
	DirectX::XMFLOAT3*		GetWorldPosition() { return &trans->GetWorldPosition; }
	DirectX::XMFLOAT3*		GetWorldRotation() { return &trans->GetWorldRotation; }


	bool					IsDirty();
	#pragma endregion 


private:
	Transform* trans; 
	DirectX::XMFLOAT3		_position;
	DirectX::XMFLOAT3		_rotation;

	bool					dirty;	
};

