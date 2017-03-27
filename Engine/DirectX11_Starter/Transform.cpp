#include "Transform.h"

using namespace DirectX;

Transform::Transform()
	:
	parent(nullptr),
	dirty(true),
	position(),
	rotation(),
	scale(1.0f, 1.0f, 1.0f)
{
	UpdateMatrix();
}

void Transform::SetPosition(float x, float y, float z)
{
	position.x = x;
	position.y = y;
	position.z = z;
	dirty = true;
}

void Transform::SetRotationEuler(float x, float y, float z)
{
	rotation.x = x;
	rotation.y = y;
	rotation.z = z;
	dirty = true;
}

void Transform::SetScale(float x, float y, float z)
{
	scale.x = x;
	scale.y = y;
	scale.z = z;
	dirty = true;
}

void Transform::SetScale(float s)
{
	scale.x = s;
	scale.y = s;
	scale.z = s;
	dirty = true;
}

DirectX::XMFLOAT4X4 * Transform::GetLocalMatrix()
{
	if (dirty)
	{
		UpdateMatrix();
	}

	return &matLocal;
}

DirectX::XMFLOAT4X4 * Transform::GetWorldMatrix()
{
	if (dirty)
	{
		UpdateMatrix();
	}
	return &matWorld;
}

void Transform::SetParent(Transform * t)
{
	if (t == this)
		t = nullptr;

	if (t != parent)
		dirty = true;

	parent = t;
}

void Transform::UpdateMatrix()
{
	XMMATRIX m = XMMatrixMultiply(
		XMMatrixMultiply(
			XMMatrixScaling(scale.x, scale.y, scale.z),
			XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z)
		),
		XMMatrixTranslation(position.x, position.y, position.z)
	);

	XMStoreFloat4x4(
		&matLocal,
		XMMatrixTranspose(m)
	);

	if (nullptr != parent)
	{

		XMMATRIX w = XMMatrixMultiply(
			m,
			XMLoadFloat4x4(parent->GetWorldMatrix())
		);

		XMStoreFloat4x4(
			&matWorld,
			XMMatrixTranspose(w)
		);
	}
	else
	{
		XMStoreFloat4x4(
			&matWorld,
			XMMatrixTranspose(m)
		);
	}

	dirty = false;
}
