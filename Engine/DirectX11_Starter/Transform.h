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

	DirectX::XMFLOAT3*		GetLocalPosition() { return &localPosition; }
	DirectX::XMFLOAT3*		GetLocalRotation() { return &localRotation; }
	DirectX::XMFLOAT3*		GetLocalScale() { return &localScale; }

	DirectX::XMFLOAT3*		GetWorldPosition() { return &worldPosition; }
	DirectX::XMFLOAT3*		GetWorldRotation() { return &worldRotation; }
	DirectX::XMFLOAT3*		GetWorldScale() { return &worldScale; }

	DirectX::XMFLOAT4X4*	GetLocalMatrix();
	DirectX::XMFLOAT4X4*	GetWorldMatrix();

	void					SetParent(Transform *t);
	void					AddChild(Transform *t);
	void					RemoveChild(Transform *t);
	Transform*				GetParent();

	bool					IsDirty();
	void					MarkDirty() { dirty = true; }
	std::vector<Transform*> children;
	void					UpdateTransform();

private:
	void					UpdateMatrix();

private:
	Transform*				parent;
	Transform*				root;
	bool					dirty;

	DirectX::XMFLOAT3		localPosition;//local
	DirectX::XMFLOAT3		localRotation;//local
	DirectX::XMFLOAT3		localScale;//local

	DirectX::XMFLOAT3		worldPosition;//world
	DirectX::XMFLOAT3		worldRotation;//world
	DirectX::XMFLOAT3		worldScale;//world

	DirectX::XMFLOAT4X4		matLocal;
	DirectX::XMFLOAT4X4		matWorld;
	DirectX::XMFLOAT4X4		localToWorldMatrix;
	
private:
	// TODO allocator for transforms;
};
