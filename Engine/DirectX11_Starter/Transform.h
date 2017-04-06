#pragma once

#include <vector>
#include <DirectXMath.h>
#include "Component.h"

class Transform : public Component
{
public:
	Transform();

	void					SetPosition(float x, float y, float z);
	void					SetRotationEuler(float x, float y, float z);
	void					SetScale(float x, float y, float z);
	void					SetScale(float s);

	DirectX::XMFLOAT3*		GetPosition() { return &position; }
	DirectX::XMFLOAT3*		GetRotation() { return &rotation; }
	DirectX::XMFLOAT3*		GetScale() { return &scale; }

	DirectX::XMFLOAT4X4*	GetLocalMatrix();
	DirectX::XMFLOAT4X4*	GetWorldMatrix();

	void					SetParent(Transform *t);
	Transform *				GetParent();

	bool					IsDirty();
	void					MarkDirty() { dirty = true; }

private:
	void					UpdateMatrix();

private:
	Transform*				parent;
	bool					dirty;
	bool					modified;

	DirectX::XMFLOAT3		position;
	DirectX::XMFLOAT3		rotation;
	DirectX::XMFLOAT3		scale;

	DirectX::XMFLOAT4X4		matLocal;
	DirectX::XMFLOAT4X4		matWorld;

private:
	// TODO allocator for transforms;
};
