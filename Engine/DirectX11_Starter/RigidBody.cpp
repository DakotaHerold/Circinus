#include "RigidBody.h"

using namespace DirectX;

RigidBody::RigidBody(Transform* t, const DirectX::BoundingBox* boxCollider)
	:
	velocity(0, 0, 0)
{
	trans = t;
	obb = new BoundingOrientedBox();
	DirectX::BoundingOrientedBox::CreateFromBoundingBox(*obb, *boxCollider);
}


RigidBody::~RigidBody()
{
	delete obb;
}

void RigidBody::Update()
{
	trans->SetWorldPosition(trans->GetWorldPosition()->x + velocity.x, trans->GetWorldPosition()->y + velocity.y, trans->GetWorldPosition()->z + velocity.z);
}

void RigidBody::SetWorldVelocity(float x, float y, float z)
{
	velocity.x = x;
	velocity.y = y;
	velocity.z = z;
}

void RigidBody::FaceTo(RigidBody* otherEntity)
{
	DirectX::XMVECTOR thisPosVec = DirectX::XMLoadFloat3(trans->GetWorldPosition());
	DirectX::XMVECTOR otherPosVec = DirectX::XMLoadFloat3(otherEntity->GetTransform()->GetWorldPosition());

	DirectX::XMVECTOR forwardVector = DirectX::XMVector3Normalize(otherPosVec - thisPosVec);

	DirectX::XMVECTOR upVector = DirectX::XMVectorSet(0, 1, 0, 0);
	DirectX::XMVECTOR tangentVector = DirectX::XMVector3Cross(upVector, forwardVector);

	if (DirectX::XMVectorGetX(DirectX::XMVector3Length(tangentVector)) < 0.001f) //check if forward is pointed at the same dir as up
	{
		upVector = DirectX::XMVectorSet(1, 0, 0, 0);
		tangentVector = DirectX::XMVector3Cross(upVector, forwardVector);
	}

	tangentVector = DirectX::XMVector3Normalize(tangentVector);
	upVector = DirectX::XMVector3Cross(forwardVector, tangentVector);

	DirectX::XMFLOAT3 forwardF3(0, 0, 0);
	XMStoreFloat3(&forwardF3, forwardVector);
	DirectX::XMFLOAT3 upF3(0, 0, 0);
	XMStoreFloat3(&upF3, upVector);
	DirectX::XMFLOAT3 tangentF3(0, 0, 0);
	XMStoreFloat3(&tangentF3, tangentVector);

	DirectX::XMFLOAT3X3 newRotationMatrixF3X3(
		upF3.y, tangentF3.y, forwardF3.y,
		upF3.x, tangentF3.x, forwardF3.x,
		upF3.z, tangentF3.z, forwardF3.z);
	/*DirectX::XMFLOAT3X3 newRotationMatrixF3X3(
		tangentF3.x, tangentF3.y, tangentF3.z,
		upF3.x, upF3.y, upF3.z,
		forwardF3.x, forwardF3.y, forwardF3.z
	);*/
	DirectX::XMMATRIX newRotationMatrix = DirectX::XMMatrixTranspose(DirectX::XMLoadFloat3x3(&newRotationMatrixF3X3));

	//DirectX::XMMATRIX newRotationMatrix = DirectX::XMMatrixLookAtRH(thisPosVec, otherPosVec, DirectX::XMVectorSet(0, 1, 0, 0));

	/*newRotationMatrix = DirectX::XMMatrixTranspose(newRotationMatrix);
	newRotationMatrix = DirectX::XMMatrixTranspose(newRotationMatrix);*/
	//DirectX::XMVECTOR dirVector = DirectX::XMQuaternionRotationMatrix(newRotationMatrix);

	//DirectX::XMFLOAT4 newRotationF4(0, 0, 0, 0);
	//DirectX::XMStoreFloat4(&newRotationF4, dirVector);
	DirectX::XMFLOAT3 dirFloat(0, 0, 0);
	//QuatToEuler(&newRotationF4, &dirFloat);
	//toEulerianAngle(&newRotationF4, &dirFloat);

	//DirectX::XMStoreFloat3(&dirFloat, dirVector);

	DirectX::XMFLOAT3X3 newRotation3X3(0, 0, 0, 0, 0, 0, 0, 0, 0);
	DirectX::XMStoreFloat3x3(&newRotation3X3, newRotationMatrix);
	dirFloat = rotate(&newRotation3X3);

	trans->SetRotationEuler(dirFloat.x, dirFloat.y, dirFloat.z);
}

void RigidBody::ProjectileShootAt(RigidBody* target, float speed) //Rotate this rigidbody and shoot it towards the target
{
	this->FaceTo(target);
	DirectX::XMFLOAT3 forward(0, 0, speed);
	DirectX::XMVECTOR localForward = DirectX::XMLoadFloat3(&forward);

	DirectX::XMVECTOR localRotationVec = DirectX::XMLoadFloat3(trans->GetLocalRotation());

	DirectX::XMVECTOR worldVelocityVec = XMVector3Rotate(localForward, DirectX::XMQuaternionRotationRollPitchYawFromVector(localRotationVec));
	DirectX::XMFLOAT3 worldVelocity;
	DirectX::XMStoreFloat3(&worldVelocity, worldVelocityVec);
}

DirectX::XMFLOAT3 RigidBody::rotate(DirectX::XMFLOAT3X3* m)
{
	XMFLOAT3 newEulerAngles(0, 0, 0);
	// Assuming the angles are in radians.
	if (m->_21 > 0.998) // singularity at north pole
	{ 
		newEulerAngles.x = std::atan2(m->_13, m->_33);
		newEulerAngles.y = DirectX::XM_PI / 2;
		newEulerAngles.z = 0;
		return newEulerAngles;
	}

	if (m->_21 < -0.998) // singularity at south pole
	{ 
		newEulerAngles.x = std::atan2(m->_13, m->_33);
		newEulerAngles.y = -DirectX::XM_PI / 2;
		newEulerAngles.z = 0;
		return newEulerAngles;
	}

	newEulerAngles.x = std::atan2(-m->_31, m->_11);
	newEulerAngles.y = std::atan2(-m->_23, m->_22);
	newEulerAngles.z = std::asin(m->_21);

	return newEulerAngles;
}

//
//DirectX::XMFLOAT4 RigidBody::LookAt(DirectX::XMVECTOR target, DirectX::XMVECTOR current, DirectX::XMVECTOR eye, DirectX::XMVECTOR up)
//{
//	// turn vectors into unit vectors 
//	DirectX::XMVECTOR n1 = DirectX::XMVector3Normalize(current - eye);
//	DirectX::XMVECTOR n2 = DirectX::XMVector3Normalize(target - eye);
//	DirectX::XMVECTOR d = DirectX::XMVector3Dot(n1, n2);
//	// if no noticable rotation is available return zero rotation
//	// this way we avoid Cross product artifacts 
//	if (DirectX::XMVectorGetX(d) > 0.9998f) return DirectX::XMFLOAT4(0, 0, 1, 0);
//	// in this case there are 2 lines on the same axis 
//	if (DirectX::XMVectorGetX(d) < -0.9998f) {
//		DirectX::XMVector3Rotate(n1, DirectX::XMQuaternionRotationRollPitchYaw(0.5f, 0, 0));
//		//n1 = n1(0.5f);
//		// there are an infinite number of normals 
//		// in this case. Anyone of these normals will be 
//		// a valid rotation (180 degrees). so rotate the curr axis by 0.5 radians this way we get one of these normals 
//	}
//	DirectX::XMVECTOR axis = n1;
//	axis = DirectX::XMVector3Cross(axis, n2);
//	DirectX::XMVECTOR pointToTarget = DirectX::XMVectorSet(1.0f + DirectX::XMVectorGetX(d), DirectX::XMVectorGetX(axis), DirectX::XMVectorGetY(axis), DirectX::XMVectorGetZ(axis));
//	pointToTarget = DirectX::XMVector3Normalize(pointToTarget);
//	// now twist around the target vector, so that the 'up' vector points along the z axis
//	double a = DirectX::XMVectorGetX(pointToTarget);
//	double b = DirectX::XMVectorGetY(pointToTarget);
//	double c = DirectX::XMVectorGetZ(pointToTarget);
//	DirectX::XMFLOAT3X3 newRotationMatrixF3X3(
//		b*b + c*c, -a*b, -a*c,
//		-b*a, a*a + c*c, -b*c,
//		-c*a, -c*b, a*a + b*b);
//	DirectX::XMMATRIX projectionMatrix = DirectX::XMLoadFloat3x3(&newRotationMatrixF3X3);
//	/*projectionMatrix.m00 = b*b + c*c;
//	projectionMatrix.m01 = -a*b;
//	projectionMatrix.m02 = -a*c;
//	projectionMatrix.m10 = -b*a;
//	projectionMatrix.m11 = a*a + c*c;
//	projectionMatrix.m12 = -b*c;
//	projectionMatrix.m20 = -c*a;
//	projectionMatrix.m21 = -c*b;
//	projectionMatrix.m22 = a*a + b*b;*/
//	//sfvec3f upProjected = projectionMatrix.transform(up);
//	//sfvec3f yaxisProjected = projectionMatrix.transform(new sfvec(0, 1, 0);
//	//d = sfvec3f.dot(upProjected, yaxisProjected);
//	//// so the axis of twist is n2 and the angle is arcos(d)
//	////convert this to quat as follows   
//	//double s = Math.sqrt(1.0 - d*d);
//	//DirectX::XMVECTOR twist = DirectX::XMVectorSet(d, n2*s, n2*s, n2*s);
//	//return sfquat.mul(pointToTarget, twist);
//	DirectX::XMFLOAT4 null;
//	return null;
//}

void RigidBody::toEulerianAngle(DirectX::XMFLOAT4 *q, DirectX::XMFLOAT3 *euler)
{
	//float ysqr = q->y * q->y;

	// roll (x-axis rotation)
	float t0 = +2.0 * (q->w * q->x + q->y * q->z);
	float t1 = +1.0 - 2.0 * (q->x * q->x + q->y * q->y);
	euler->x = std::atan2(t0, t1);

	// pitch (y-axis rotation)
	float t2 = +2.0 * (q->w * q->y - q->z * q->x);
	/*t2 = t2 > 1.0 ? 1.0 : t2;
	t2 = t2 < -1.0 ? -1.0 : t2;*/
	euler->y = std::asin(t2);

	// yaw (z-axis rotation)
	float t3 = +2.0 * (q->w * q->z + q->x * q->y);
	float t4 = +1.0 - 2.0 * (q->y * q->y + q->z * q->z);
	euler->z = std::atan2(t3, t4);
}

///////////////////////////////////////////////////////////////////////////////
// Function:	QuatToEuler
// Purpose:		Convert a Quaternion back to Euler Angles
// Arguments:	Quaternions and target Euler vector
// Notes:		The method is to convert Quaternion to a 3x3 matrix and
//				decompose the matrix.  This is subject to the
//				ambiguities of square roots and problems with inverse trig.
//				Matrix to Euler conversion is really very ill-defined but works
//				for my purposes.
///////////////////////////////////////////////////////////////////////////////
void RigidBody::QuatToEuler(DirectX::XMFLOAT4 *quat, DirectX::XMFLOAT3 *euler)
{
	/// Local Variables ///////////////////////////////////////////////////////////
	float matrix[3][3];
	float cx, sx, x;
	float cy, sy, y, yr;
	float cz, sz, z;
	///////////////////////////////////////////////////////////////////////////////
	// CONVERT QUATERNION TO MATRIX - I DON'T REALLY NEED ALL OF IT
	matrix[0][0] = 1.0f - (2.0f * quat->y * quat->y) - (2.0f * quat->z * quat->z);
	//	matrix[0][1] = (2.0f * quat->x * quat->y) - (2.0f * quat->w * quat->z);
	//	matrix[0][2] = (2.0f * quat->x * quat->z) + (2.0f * quat->w * quat->y);
	matrix[1][0] = (2.0f * quat->x * quat->y) + (2.0f * quat->w * quat->z);
	//	matrix[1][1] = 1.0f - (2.0f * quat->x * quat->x) - (2.0f * quat->z * quat->z);
	//	matrix[1][2] = (2.0f * quat->y * quat->z) - (2.0f * quat->w * quat->x);
	matrix[2][0] = (2.0f * quat->x * quat->z) - (2.0f * quat->w * quat->y);
	matrix[2][1] = (2.0f * quat->y * quat->z) + (2.0f * quat->w * quat->x);
	matrix[2][2] = 1.0f - (2.0f * quat->x * quat->x) - (2.0f * quat->y * quat->y);

	sy = -matrix[2][0];
	cy = sqrt(1 - (sy * sy));
	yr = (float)atan2(sy, cy);
	euler->y = (yr * 180.0f) / (float)DirectX::XM_PI;

	// AVOID DIVIDE BY ZERO ERROR ONLY WHERE Y= +-90 or +-270 
	// NOT CHECKING cy BECAUSE OF PRECISION ERRORS
	if (sy != 1.0f && sy != -1.0f)
	{
		cx = matrix[2][2] / cy;
		sx = matrix[2][1] / cy;
		euler->x = ((float)atan2(sx, cx) * 180.0f) / (float)DirectX::XM_PI;	// RAD TO DEG

		cz = matrix[0][0] / cy;
		sz = matrix[1][0] / cy;
		euler->z = ((float)atan2(sz, cz) * 180.0f) / (float)DirectX::XM_PI;	// RAD TO DEG
	}
	else
	{
		// SINCE Cos(Y) IS 0, I AM SCREWED.  ADOPT THE STANDARD Z = 0
		// I THINK THERE IS A WAY TO FIX THIS BUT I AM NOT SURE.  EULERS SUCK
		// NEED SOME MORE OF THE MATRIX TERMS NOW
		matrix[1][1] = 1.0f - (2.0f * quat->x * quat->x) - (2.0f * quat->z * quat->z);
		matrix[1][2] = (2.0f * quat->y * quat->z) - (2.0f * quat->w * quat->x);
		cx = matrix[1][1];
		sx = -matrix[1][2];
		euler->x = ((float)atan2(sx, cx) * 180.0f) / (float)DirectX::XM_PI;	// RAD TO DEG

		cz = 1.0f;
		sz = 0.0f;
		euler->z = ((float)atan2(sz, cz) * 180.0f) / (float)DirectX::XM_PI;	// RAD TO DEG
	}
}
// QuatToEuler  ///////////////////////////////////////////////////////////////

void RigidBody::SetEntity(int id)
{
	this->Component::SetEntity(id);
	// TODO: Update with transform position
}

bool RigidBody::SphereCollisionCheck(RigidBody *otherRbody)
{
	const BoundingOrientedBox &box1 = *(obb);
	BoundingSphere sphere1;
	BoundingSphere::CreateFromBoundingBox(sphere1, box1);
	XMFLOAT3 rBody1Position = *(trans->GetLocalPosition());
	sphere1.Center.x += rBody1Position.x;
	sphere1.Center.y += rBody1Position.y;
	sphere1.Center.z += rBody1Position.z;

	const BoundingOrientedBox &box2 = *(otherRbody->obb);
	BoundingSphere sphere2;
	BoundingSphere::CreateFromBoundingBox(sphere2, box2);
	XMFLOAT3 rBody2Position = *(otherRbody->GetTransform()->GetLocalPosition());
	sphere2.Center.x += rBody2Position.x;
	sphere2.Center.y += rBody2Position.y;
	sphere2.Center.z += rBody2Position.z;

	return sphere1.Intersects(sphere2);
}

bool RigidBody::BoxCollisionCheck(RigidBody * otherRbody)
{
	XMFLOAT3* pos1 = trans->GetLocalPosition();
	obb->Center.x = pos1->x;
	obb->Center.x = pos1->y;
	obb->Center.x = pos1->z;

	XMFLOAT3* pos2 = otherRbody->trans->GetLocalPosition();
	otherRbody->obb->Center.x = pos2->x;
	otherRbody->obb->Center.y = pos2->y;
	otherRbody->obb->Center.z = pos2->z;

	return obb->Intersects(*(otherRbody->obb));
}
