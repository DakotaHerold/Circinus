#pragma once
#include "Mesh.h"
#include "Material.h"
#include "Component.h"
#include <utility>

using namespace DirectX; 

typedef std::size_t eid;

class Entity
{
public:
	~Entity();
	Entity(Mesh* inputMeshm, Material* inputMaterial); 
	//attributes 
	Mesh* mesh;
	Material* material; 

	//getters and setters
	XMFLOAT3 GetPosition() { return position; }
	XMFLOAT3 GetRotation() { return rotation;  }
	XMFLOAT3 GetScale() { return this->scale; }
	XMFLOAT4X4* GetWorldMatrix() { return &worldMatrix;  }

	void SetWorldMatrix(XMFLOAT4X4 newWorldMatrix) { worldMatrix = newWorldMatrix; }
	void SetPosition(float x, float y, float z) { position = XMFLOAT3(x, y, z); }
	void SetRotation(float x, float y, float z) { rotation = XMFLOAT3(x, y, z); }
	void SetScale(float x, float y, float z) { scale = XMFLOAT3(x, y, z); }

	//Class Specific functions 
	void updateScene(); 
	void drawScene(ID3D11DeviceContext* deviceContext);
	void drawDeferred(ID3D11DeviceContext* deferredContext, ID3D11CommandList* commandList);
	void Move(float x, float y, float z) { position.x += x;	position.y += y;	position.z += z; }
	void Rotate(float x, float y, float z) { rotation.x += x;	rotation.y += y;	rotation.z += z; }
	void Scale(float x, float y, float z) { scale.x += x;	scale.y += y;	scale.z += z; }
	void prepareMaterial(XMFLOAT4X4& view, XMFLOAT4X4& proj); 

	void Update();

public :
	
	Entity();

	template <typename T, typename... Args>
	T* AddComponent(Args&&... args);

	/// Removes a component
	/// \tparam The type of component you wish to remove
	template <typename T>
	bool RemoveComponent();

	/// Removes all the components attached to the Entity
	void RemoveAllComponents();

	/// Retrives a component from this Entity
	/// \tparam The type of component you wish to retrieve
	/// \return A pointer to the component
	template <typename T>
	T* GetComponent() const;

	/// Determines if this Entity has a component or not
	/// \tparam The type of component you wish to check for
	/// \return true if this Entity contains a component
	template <typename T>
	bool HasComponent() const;

private :
	eid id;

	void AddComponent(Component* component, TypeId componentTypeId);
	bool RemoveComponent(TypeId componentTypeId);
	Component * GetComponent(TypeId componentTypeId) const;
	bool HasComponent(TypeId componentTypeId) const;

private:
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 rotation;
	DirectX::XMFLOAT3 scale;
	XMFLOAT4X4 worldMatrix;

	vector<pair<TypeId, Component*>> allComponets;
};

template <typename T, typename... Args>
T* Entity::AddComponent(Args&&... args)
{
	static_assert(std::is_base_of<Component, T>(), "T is not a component, cannot add T to entity");
	// TODO: align components by type
	auto component = new T{ std::forward<Args>(args)... };
	AddComponent(component, ComponentTypeId<T>());
	return component;
}

template <typename T>
bool Entity::RemoveComponent()
{
	static_assert(std::is_base_of<Component, T>(), "T is not a component, cannot remove T from entity");
	return RemoveComponent(ComponentTypeId<T>());
}

template <typename T>
T* Entity::GetComponent() const
{
	static_assert(std::is_base_of<Component, T>(), "T is not a component, cannot retrieve T from entity");
	return static_cast<T*>(GetComponent(ComponentTypeId<T>()));
}

// TODO: Get all component

template <typename T>
bool Entity::HasComponent() const
{
	static_assert(std::is_base_of<Component, T>(), "T is not a component, cannot determine if entity has T");
	return HasComponent(ComponentTypeId<T>());
}