#include "Entity.h"

Entity::~Entity()
{
	//delete components
	for (auto p : allComponets)
	{
		delete p.second;
	}
	allComponets.clear();
}

Entity::Entity() {
	// TODO: Lock for parallel
	static eid nextIndex = 0;

	id = nextIndex++;
}

Entity::Entity(Mesh * inputMesh, Material* inputMaterial)
{
	mesh = inputMesh; 
	material = inputMaterial; 
	XMStoreFloat4x4(&worldMatrix, XMMatrixIdentity());
	position = XMFLOAT3(0, 0, 0);
	rotation = XMFLOAT3(0, 0, 0);
	scale = XMFLOAT3(1, 1, 1);
}

void Entity::updateScene()
{
	//mtx.lock();
	//update mesh if at all
	//Update world matrix by multiplying matrices 
	XMMATRIX trans = XMMatrixTranslation(position.x, position.y, position.z);
	XMMATRIX rotX = XMMatrixRotationX(rotation.x);
	XMMATRIX rotY = XMMatrixRotationY(rotation.y);
	XMMATRIX rotZ = XMMatrixRotationZ(rotation.z);
	XMMATRIX sc = XMMatrixScaling(scale.x, scale.y, scale.z);

	XMMATRIX total = sc * rotZ * rotY * rotX * trans;
	XMStoreFloat4x4(&worldMatrix, XMMatrixTranspose(total));
	//mtx.unlock();
}

void Entity::drawScene(ID3D11DeviceContext * deviceContext)
{
	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	ID3D11Buffer* tempBuffer = this->mesh->GetVertexBuffer();

	deviceContext->IASetVertexBuffers(0, 1, &tempBuffer, &stride, &offset);
	deviceContext->IASetIndexBuffer(this->mesh->GetIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);

	// Finally do the actual drawing
	//  - Do this ONCE PER OBJECT you intend to draw
	//  - This will use all of the currently set DirectX "stuff" (shaders, buffers, etc)
	//  - DrawIndexed() uses the currently set INDEX BUFFER to look up corresponding
	//     vertices in the currently set VERTEX BUFFER
	deviceContext->DrawIndexed(
		this->mesh->GetIndexCount(),     // The number of indices to use (we could draw a subset if we wanted)
		0,     // Offset to the first index we want to use
		0);    // Offset to add to each index when looking up vertices
}

void Entity::drawDeferred(ID3D11DeviceContext * deferredContext, ID3D11CommandList* commandList)
{
	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	ID3D11Buffer* tempBuffer = this->mesh->GetVertexBuffer();

	deferredContext->IASetVertexBuffers(0, 1, &tempBuffer, &stride, &offset);
	deferredContext->IASetIndexBuffer(this->mesh->GetIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);

	// Finally do the actual drawing
	//  - Do this ONCE PER OBJECT you intend to draw
	//  - This will use all of the currently set DirectX "stuff" (shaders, buffers, etc)
	//  - DrawIndexed() uses the currently set INDEX BUFFER to look up corresponding
	//     vertices in the currently set VERTEX BUFFER
	deferredContext->DrawIndexed(
		this->mesh->GetIndexCount(),     // The number of indices to use (we could draw a subset if we wanted)
		0,     // Offset to the first index we want to use
		0);    // Offset to add to each index when looking up vertices

	// Add rendering code to command list 
	//deferredContext->FinishCommandList
	deferredContext->FinishCommandList(FALSE, &commandList);

	
}

void Entity::prepareMaterial(XMFLOAT4X4& view, XMFLOAT4X4& proj)
{
	XMFLOAT4X4 worldMatrixIT;
	XMStoreFloat4x4(&worldMatrixIT, XMMatrixInverse(nullptr, XMMatrixTranspose(XMLoadFloat4x4(&worldMatrix))));

	//Prepares material object for reuse
	material->vertexShader->SetMatrix4x4("matWorld", worldMatrix);
	material->vertexShader->SetMatrix4x4("matWorld_IT", worldMatrixIT);

	// TODO move these outside
	material->vertexShader->SetMatrix4x4("matView", view); 
	material->vertexShader->SetMatrix4x4("matProj", proj);
	if (nullptr != material->texDiffuse)
	{
		material->pixelShader->SetShaderResourceView("texDiffuse", material->texDiffuse->GetSRV());
	}
	if (nullptr != material->sampBasic)
	{
		material->pixelShader->SetSamplerState("sampBasic", material->sampBasic);
	}


	material->vertexShader->SetShader(true); 
	material->pixelShader->SetShader(true); 
}



//bool Entity::AddComponent(Component* com)
//{
//	vector<Component*>::iterator itr = allComponets.begin();
//	int i = 0;
//	for (unsigned i = 0; i < allComponets.size(); i++) {
//		if (allComponets[i]->GetType() == com->GetType()) {
//			return false;
//		}
//	}
//	com->SetEntity(this);
//	allComponets.push_back(com);
//	return true;
//}
//
//bool Entity::RemoveComponent(ComponentType type)
//{	
//	int i = 0;
//	for (unsigned i = 0; i < allComponets.size();i++) {
//		if (allComponets[i]->GetType() == type) {
//			allComponets.erase(allComponets.begin() + i);
//			return true;
//		}
//	}
//	return false;
//}

//Component * Entity::GetComponent(ComponentType type)
//{
//	for (int i = 0; i < allComponets.size(); i++) {
//		if (allComponets[i]->GetType() == type) {
//			return allComponets[i];
//		}
//	}
//	return nullptr;
//}

void Entity::AddComponent(Component* component, TypeId componentTypeId)
{
	//getWorld().m_entityAttributes.componentStorage.addComponent(*this, component, componentTypeId);

	component->SetEntity(id);
	allComponets.push_back(make_pair(componentTypeId, component));
}

bool Entity::RemoveComponent(TypeId componentTypeId)
{
	//getWorld().m_entityAttributes.componentStorage.removeComponent(*this, componentTypeId);
	
	for (int i = 0; i < allComponets.size(); i++) {
		if (allComponets[i].first == componentTypeId) {
			allComponets.erase(allComponets.begin() + i);
			return true;
		}
	}
	return false;
}

Component * Entity::GetComponent(TypeId componentTypeId) const
{
	for (auto p : allComponets) {
		if (p.first == componentTypeId) {
			return p.second;
		}
	}
	return nullptr;

	//return getWorld().m_entityAttributes.componentStorage.getComponent(*this, componentTypeId);
}

bool Entity::HasComponent(TypeId componentTypeId) const
{
	for (auto p : allComponets) {
		if (p.first == componentTypeId) {
			return true;
		}
	}
	return false;
}

void Entity::RemoveAllComponents()
{
	/*getWorld().m_entityAttributes.componentStorage.removeAllComponents(*this);*/
}


