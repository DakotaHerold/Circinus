#pragma once
#include "Component.h"
#include <DirectXMath.h>

class RigidBody :
	public Component
{
public:
	RigidBody();
	~RigidBody();

	void SetEntity(Entity* entity) override;

	//void					SetPosition(float x, float y, float z);
	void					SetPosition(DirectX::XMFLOAT3 position);
	void					SetRotationEuler(float x, float y, float z);

	DirectX::XMFLOAT3*		GetPosition() { return &_position; }
	DirectX::XMFLOAT3*		GetRotation() { return &_rotation; }

	bool					IsDirty();
private:
	DirectX::XMFLOAT3		_position;
	DirectX::XMFLOAT3		_rotation;

	bool					dirty;	
};

