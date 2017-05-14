#include "Transform.h"
#include "ComponentManager.h"
using namespace DirectX;

Transform::Transform()
	:
	parent(nullptr),
	dirty(true),
	localPosition(0, 0, 0),
	localRotation(0, 0, 0),
	localScale(1.0f, 1.0f, 1.0f)
{
	if (ComponentManager::current != nullptr) {
		root = ComponentManager::current->root;
		parent = root;
		root->AddChild(this);
	}
	else {
		SetEntity(-1);
	}
}

Transform::~Transform()
{

}

void Transform::SetWorldPosition(float x, float y, float z)
{
	worldPosition.x = x;
	worldPosition.y = y;
	worldPosition.z = z;

	XMMATRIX world = XMMatrixMultiply(
		XMMatrixMultiply(
			XMMatrixScaling(localScale.x, localScale.y, localScale.z),
			XMMatrixRotationRollPitchYaw(localRotation.x, localRotation.y, localRotation.z)
		),
		XMMatrixTranslation(worldPosition.x, worldPosition.y, worldPosition.z)
	);
	XMMATRIX local = XMMatrixMultiply(
		world,
		XMMatrixInverse(nullptr,
			XMMatrixTranspose(
				XMLoadFloat4x4(parent->GetWorldMatrix())
			)
		)
	);

	XMStoreFloat4x4(
		&matLocal,
		XMMatrixTranspose(local)
	);

	SetLocalPosition(matLocal._14, matLocal._24, matLocal._34);
	
	dirty = true;
}

void Transform::SetLocalPosition(float x, float y, float z)
{
	localPosition.x = x;
	localPosition.y = y;
	localPosition.z = z;
	dirty = true;
}

void Transform::SetRotationEuler(float x, float y, float z)
{
	localRotation.x = x;
	localRotation.y = y;
	localRotation.z = z;
	dirty = true;
}

void Transform::SetScale(float x, float y, float z)
{
	localScale.x = x;
	localScale.y = y;
	localScale.z = z;
	dirty = true;
}

void Transform::SetScale(float s)
{
	localScale.x = s;
	localScale.y = s;
	localScale.z = s;
	dirty = true;
}

DirectX::XMFLOAT4X4 * Transform::GetLocalMatrix()
{
	return &matLocal;
}

DirectX::XMFLOAT4X4 * Transform::GetWorldMatrix()
{
	return &matWorld;
}

void Transform::SetParent(Transform * t)
{
	if (parent == t)
	{
		return;
	}

	if (t == this || t == nullptr) {
		if (parent == root) {
			return;
		}
		else {
			parent->RemoveChild(this);
			parent = root;
			parent->AddChild(this);
		}
	}
	else {
		parent->RemoveChild(this);
		parent = t;
		parent->AddChild(this);
	}

	dirty = true;
}

void Transform::AddChild(Transform * t)
{
	children.push_back(t);
}

void Transform::RemoveChild(Transform * t)
{
	for (size_t i = 0; i < children.size(); i++) {
		if (children[i]->GetID() == t->GetID()) {
			children.erase(children.begin() + i);
			return;
		}
	}
}

Transform * Transform::GetParent()
{
	return parent;
}

bool Transform::IsDirty()
{
	return dirty;
}

void Transform::UpdateMatrix()
{
	XMMATRIX m = XMMatrixMultiply(
		XMMatrixMultiply(
			XMMatrixScaling(localScale.x, localScale.y, localScale.z),
			XMMatrixRotationRollPitchYaw(localRotation.x, localRotation.y, localRotation.z)
		),
		XMMatrixTranslation(localPosition.x, localPosition.y, localPosition.z)
	);

	XMStoreFloat4x4(
		&matLocal,
		XMMatrixTranspose(m)
	);
	if (parent != nullptr) {
		XMMATRIX w = XMMatrixMultiply(
			m,
			XMMatrixTranspose(
				XMLoadFloat4x4(parent->GetWorldMatrix())
			)
		);
		XMStoreFloat4x4(
			&matWorld,
			XMMatrixTranspose(w)
		);
	}
	else
	{
		matWorld = matLocal;
	}
	worldPosition.x = matWorld._14;
	worldPosition.y = matWorld._24;
	worldPosition.z = matWorld._34;
	dirty = false;
}

void Transform::UpdateTransform()
{
	bool wasDirty = dirty;
	if (dirty) {
		UpdateMatrix();
	}
	for (size_t i = 0; i < children.size(); i++) {
		if (wasDirty) {
			children[i]->dirty = true;
		}
		children[i]->UpdateTransform();
	}
}

void Transform::StartSerialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer)
{
	if (GetEntityID() != -1) {
		Entity* entity = GetEntity();
		entity->Serialize(writer);		
	}	
	for (Transform* t : children) {
		t->StartSerialize(writer);
	}
}

void Transform::Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer)
{
	writer.StartObject();
	writer.String("name");
	writer.String("Transform");
	writer.String("parent");
	if (parent->GetEntityID() == -1) {
		writer.String("root");
	}
	else {
		writer.String(parent->GetEntity()->GetName().c_str());
	}	
	writer.String("PositionX");
	writer.Double(localPosition.x);
	writer.String("PositionY");
	writer.Double(localPosition.y);
	writer.String("PositionZ");
	writer.Double(localPosition.z);
	writer.String("RotationX");
	writer.Double(localRotation.x);
	writer.String("RotationY");
	writer.Double(localRotation.y);
	writer.String("RotationZ");
	writer.Double(localRotation.z);
	writer.String("ScaleX");
	writer.Double(localScale.x);
	writer.String("ScaleY");
	writer.Double(localScale.y);
	writer.String("ScaleZ");
	writer.Double(localScale.z);
	writer.EndObject();
}

void Transform::Load(rapidjson::Value v)
{
	SetLocalPosition(v["PositionX"].GetFloat(), v["PositionY"].GetFloat(), v["PositionZ"].GetFloat());
	SetScale(v["ScaleX"].GetFloat(), v["ScaleY"].GetFloat(), v["ScaleZ"].GetFloat());
	SetRotationEuler(v["RotationX"].GetFloat(), v["RotationY"].GetFloat(), v["RotationZ"].GetFloat());	
}
