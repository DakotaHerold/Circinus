#include "Entity.h"



Entity::Entity()
{
}


Entity::~Entity()
{
}

Entity::Entity(Mesh * inputMesh, Material* inputMaterial)
{
	mesh = inputMesh; 
	material = inputMaterial; 
	position = XMFLOAT4X4(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);
	rotation = XMFLOAT4X4(
		1.0f, 0.0f, 0.0f, 0.0f, 
		0.0f, 1.0f, 0.0f, 0.0f, 
		0.0f, 0.0f, 1.0f, 0.0f , 
		0.0f, 0.0f, 0.0f, 1.0f);
	scalar = XMFLOAT4X4(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);
	
	worldMatrix = XMFLOAT4X4(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);
	/*XMMATRIX W = XMMatrixIdentity();
	XMStoreFloat4x4(&worldMatrix, XMMatrixTranspose(W));*/
}

XMFLOAT4X4 Entity::getPosition()
{
	return position;
}

XMFLOAT4X4 Entity::getRotation()
{
	return rotation;
}

XMFLOAT4X4 Entity::getScale()
{
	return scalar;
}

XMFLOAT4X4 Entity::getWorldMatrix()
{
	return worldMatrix;
}

void Entity::setPosition(XMFLOAT3 newPos)
{
	position = XMFLOAT4X4
		(newPos.x, position._12, position._13, position._14,
			position._21, newPos.y, position._23, position._24,
			position._31, position._32, newPos.z, position._34,
			position._41, position._42, position._43, position._44); 
}

void Entity::setRotation(XMFLOAT4 newRot)
{
	rotation = XMFLOAT4X4
		(newRot.x, rotation._12, rotation._13, rotation._14,
			rotation._21, newRot.y, rotation._23, rotation._24,
			rotation._31, rotation._32, newRot.z, rotation._34,
			rotation._41, rotation._42, rotation._43, newRot.w);
}

void Entity::setScale(XMFLOAT3 newScale)
{
	scalar = XMFLOAT4X4
		(newScale.x * scalar._11, scalar._12, scalar._13, scalar._14,
			scalar._21, newScale.y * scalar._23, scalar._23, scalar._24,
			scalar._31, scalar._32, newScale.z * scalar._33, scalar._34,
			scalar._41, scalar._42, scalar._43, scalar._44);
}

void Entity::setWorldMatrix(XMFLOAT4X4 newWorldMatrix)
{
	worldMatrix = newWorldMatrix;
}

void Entity::updateScene()
{
	//update mesh if at all
	
	
	//Update world matrix by multiplying matrices 
	//Load Matrices for operations
	XMMATRIX worldMat = XMLoadFloat4x4(&worldMatrix); 
	XMMATRIX translation = XMLoadFloat4x4(&position);
	XMMATRIX rot = XMLoadFloat4x4(&rotation);
	XMMATRIX scaling = XMLoadFloat4x4(&scalar);

	worldMat = translation * rot * scaling; 
	XMStoreFloat4x4(&worldMatrix, XMMatrixTranspose(worldMat)); 

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



void Entity::move(XMFLOAT4 force)
{
	//Would doing something like this be more optimized?
	/*position = XMFLOAT4X4(
		position._11 += force.x, position._12, position._13, position._14,
		position._21, position._22 += force.y, position._23, position._24,
		position._31, position._32, position._33 += force.z, position._34,
		position._41, position._42, position._43, position._44 += force.w); */

	XMMATRIX matrix = XMLoadFloat4x4(&position); 

	matrix *= XMMatrixTranslation(force.x, force.y, force.z); 

	XMStoreFloat4x4(&position, matrix); 


}

void Entity::scale(XMFLOAT4 nscalar)
{
	//Would doing something like this be more optimized?
	/*scalar = XMFLOAT4X4(
		scalar._11 , scalar._12, scalar._13, nscalar.x,
		scalar._21, scalar._22, position._23, nscalar.y,
		scalar._31, scalar._32, scalar._33, nscalar.z,
		scalar._41, scalar._42, scalar._43, scalar._44 * 1);*/

	XMMATRIX matrix = XMLoadFloat4x4(&scalar);

	matrix *=  XMMatrixScaling(nscalar.x, nscalar.y, nscalar.z);

	XMStoreFloat4x4(&scalar, matrix);

	
}

void Entity::rotate(XMFLOAT4 axis, float angle)
{

	//Would doing something like this be more optimized?
	/*rotation = XMFLOAT4X4(
		rotation._11 + nrotation.x, rotation._12, rotation._13, rotation._14,
		rotation._21, rotation._22 + nrotation.y, rotation._23, rotation._24,
		rotation._31, rotation._32, rotation._33 + nrotation.z, rotation._34,
		rotation._41, rotation._42, rotation._43, rotation._44 + nrotation.w);*/

	XMMATRIX matrix = XMLoadFloat4x4(&rotation); 
	XMVECTOR rotAxis = XMLoadFloat4(&axis); 

	matrix *= XMMatrixRotationAxis(rotAxis, angle); 

	XMStoreFloat4x4(&rotation, matrix);

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


