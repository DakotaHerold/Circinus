
#include "Entity.h"
#include "Main.h"



Entity::Entity()
{
}


Entity::~Entity()
{
	collider = nullptr; 
	motionState = nullptr; 
	delete motionState;
	delete collider;
	collisionShape = nullptr; 
	delete collisionShape;

}


// Helper functions 

// Converts an XMFLOAT3 to a btVector3 -- Richard Selneck  
static inline btVector3 XMtoBT(const XMFLOAT3& xm)
{
	return btVector3(xm.x, xm.y, xm.z);
}

// Converts a btVector3 to an XMFLOAT3 -- Richard Selneck 
static inline XMFLOAT3 BTtoXM(const btVector3& bt)
{
	return DirectX::XMFLOAT3(bt.getX(), bt.getY(), bt.getZ());
}

// Copies Bullet's transform to our transform
void Entity::CopyTransformFromBullet()
{
	// Get the two transforms
	btTransform  btTrans;
	motionState->getWorldTransform(btTrans);
	

	// Copy the position
	XMFLOAT3 position = BTtoXM(btTrans.getOrigin());
	setPosition(position.x, position.y, position.z);

	//Copy the rotation
	//btQuaternion btRot = btTrans.getRotation();
	//float x = btRot.getX(); 
	//float y = btRot.getY(); 
	//float z = btRot.getZ(); 
	//float w = btRot.getW(); 
	//XMFLOAT4 xmRot = XMFLOAT4(x,y,z,w); 
	//
	//XMFLOAT4X4 newMat;
	////XMMATRIX tempMat = XMLoadFloat4x4(&newMat); 
	//XMVECTOR vec = XMLoadFloat4(&xmRot); 
	//XMMATRIX mat = XMMatrixTranslationFromVector(vec); 
	//

	//XMStoreFloat4x4(&newMat, mat);


	//setRotation( newMat );
}

// Copies our transform to Bullet's transform
void Entity::CopyTransformToBullet()
{
	// Get the two transforms
	btTransform  btTrans;
	motionState->getWorldTransform(btTrans);


	XMFLOAT3 currentPos = XMFLOAT3(position._14, position._24, position._34); 

	// Copy the position
	btTrans.setOrigin(XMtoBT(currentPos));

	//// Copy the rotation
	//XMFLOAT4 xmRot = XMFLOAT4(rotation._11, rotation._22, rotation._33, rotation._44);
	//btQuaternion btRot(xmRot.x, xmRot.y, xmRot.z, xmRot.w);
	//btTrans.setRotation(btRot);

	// Now set the transform
	motionState->setWorldTransform(btTrans);
	collider->setWorldTransform(btTrans);
}

Entity::Entity(Mesh * inputMesh, Material* inputMaterial, btCollisionShape* colliderShape, btDefaultMotionState* mState, btRigidBody* colliderTemp)
{
	//Phsyics 
	collisionShape = colliderShape;
	motionState = mState;
	collider = colliderTemp;
	//collider->setCcdMotionThreshold(1);
	//collider->setCcdSweptSphereRadius(0.2f); 
	//Main::dynamicsWorld->addRigidBody(colliderTemp);

	//Find position
	btTransform trans; 
	motionState->getWorldTransform(trans);
	btVector3 pos = trans.getOrigin();
	btMatrix3x3 matrix = trans.getBasis(); 

	float x = pos.getX();
	float y = pos.getY();
	float z = pos.getZ();

	
	
	// Graphics 
	mesh = inputMesh;
	material = inputMaterial;
	position = XMFLOAT4X4(
		1.0f, 0.0f, 0.0f, x,
		0.0f, 1.0f, 0.0f, y,
		0.0f, 0.0f, 1.0f, z,
		0, 0,0, 1.0f);
	rotation = XMFLOAT4X4(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
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

	//move(XMFLOAT4(x, y, z, 1.0f)); 
	//trans.setIdentity(); 
	//trans.setOrigin(btVector3(position._11, position._22, position._33));
	//trans.setOrigin(btVector3(position._14, position._22, position._33));
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

void Entity::setPosition(float x, float y, float z)
{
	/*position = XMFLOAT4X4(
		1.0f, 0.0f, 0.0f, x,
		0.0f, 1.0f, 0.0f, y,
		0.0f, 0.0f, 1.0f, z, 
		0.0f, 0.0f, 0.0f, 1.0f);*/
	position = XMFLOAT4X4(
		1.0f, 0.0f, 0.0f, 0,
		0.0f, 1.0f, 0.0f, 0,
		0.0f, 0.0f, 1.0f, 0,
		x, y, z, 1.0f);
}

void Entity::setRotation(XMFLOAT4X4 newRot)
{
	rotation = newRot; 
	/*XMMATRIX matOne = XMLoadFloat4x4(&rotation); 
	XMMATRIX matTwo = XMLoadFloat4x4(&newRot); 
	XMVECTOR vec = XMQuaternionRotationMatrix(matTwo); 
	matOne = XMMatrixIdentity(); 
	matOne *= XMMatrixRotationRollPitchYawFromVector(vec); 
	XMStoreFloat4x4(&rotation, matOne); */
	//rotation = newRot;
}



void Entity::setScale(XMFLOAT3 newScale)
{
	scalar = XMFLOAT4X4
		(newScale.x , 0, 0, 0,
			0, newScale.y , 0, 0,
			0, 0, newScale.z , 0,
			0, 0, 0, 1);
	/*
	scalar = XMFLOAT4X4
		(newScale.x * scalar._11, scalar._12, scalar._13, scalar._14,
			scalar._21, newScale.y * scalar._23, scalar._23, scalar._24,
			scalar._31, scalar._32, newScale.z * scalar._33, scalar._34,
			scalar._41, scalar._42, scalar._43, scalar._44);
			*/
}

void Entity::setWorldMatrix(XMFLOAT4X4 newWorldMatrix)
{
	worldMatrix = newWorldMatrix;
}

void Entity::updateScene()
{
	
	//update mesh if at all
	//Find position
	/*btTransform trans = collider->getWorldTransform();
	btVector3 pos = trans.getOrigin();
	float x = pos.getX(); 
	float y = pos.getY(); 
	float z = pos.getZ(); */

	btTransform trans = collider->getWorldTransform(); 
	//collider->getMotionState()->getWorldTransform(trans); 

	btVector3 newPos = trans.getOrigin(); 
	float x = newPos.getX(); 
	float y = newPos.getY(); 
	float z = newPos.getZ(); 
	//move(XMFLOAT4(x,y,z, 1.0f)); 

	
	/*btMatrix3x3 matrix = trans.getBasis();
	btVector3 rowOne = matrix.getRow(0); 
	btVector3 rowTwo = matrix.getRow(1);
	btVector3 rowThree = matrix.getRow(2);
	XMFLOAT4X4 mat = XMFLOAT4X4(
		1, 0, 0 ,rowOne.getX(),
		0, 1, 0, rowTwo.getY(),
		0, 0, 1, rowThree.getZ(),
		0, 0, 0, 1.0f
		);
	XMMATRIX translation = XMLoadFloat4x4(&mat); */
	
	
	
	//Update world matrix by multiplying matrices 
	//Load Matrices for operations
	XMMATRIX worldMat = XMLoadFloat4x4(&worldMatrix);
	XMMATRIX translation = XMLoadFloat4x4(&position);
	XMMATRIX rot = XMLoadFloat4x4(&rotation);
	XMMATRIX scaling = XMLoadFloat4x4(&scalar);

	worldMat = translation * rot * scaling;
	
	XMStoreFloat4x4(&worldMatrix, XMMatrixTranspose(worldMat));
	
	//btTransform t= collider->getWorldTransform();
	//trans.setIdentity();
	//trans.setOrigin(btVector3(position._14, position._24, position._34));
	//t.setOrigin(btVector3(position._41, position._42, position._43));
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


	CopyTransformToBullet(); 
	//CopyTransformFromBullet();

	// add back to physics 
	/*btTransform trans = collider->getWorldTransform();
	XMFLOAT4X4 currentPos = getPosition(); 
	float x = currentPos._11;
	float y = currentPos._22;
	float z = currentPos._33;
	XMFLOAT4X4 currentRot = getRotation();
	float xRot = currentRot._11;
	float yRot = currentRot._22;
	float zRot = currentRot._33;
	float w = currentRot._44;
	trans.setIdentity();
	trans.setOrigin(btVector3(x , y, z));
	trans.setRotation(btQuaternion(xRot,yRot,zRot,w));
	collider->setWorldTransform(trans);*/
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

	matrix *= XMMatrixScaling(nscalar.x, nscalar.y, nscalar.z);

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
	material->pixelShader->SetShaderResourceView("diffuseTexture", material->SRV);
	material->pixelShader->SetSamplerState("trillinear", material->samplerState);
}


