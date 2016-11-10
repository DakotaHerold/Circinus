#include "Entity.h"

Entity::~Entity()
{
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

void Entity::prepareMaterial(XMFLOAT4X4 view, XMFLOAT4X4 proj)
{
	//Prepares material object for reuse 
	material->vertexShader->SetMatrix4x4("world", worldMatrix); 
	material->vertexShader->SetMatrix4x4("view", view); 
	material->vertexShader->SetMatrix4x4("projection", proj); 
	material->vertexShader->SetShader(true); 
	material->pixelShader->SetShader(true); 
}


