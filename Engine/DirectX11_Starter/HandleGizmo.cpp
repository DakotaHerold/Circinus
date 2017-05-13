#include "HandleGizmo.h"

#include "ComponentManager.h"
#include "InputManager.h"
#include "GizmoRenderer.h"
#include "Entity.h"
#include "Transform.h"


using namespace DirectX;

void _vectorcall HandleGizmo::Update(
	Entity* entity,
	float deltaTime,
	float totalTime,
	DirectX::FXMVECTOR rayStart,
	DirectX::FXMVECTOR rayDir, 
	DirectX::FXMVECTOR mouseWorldDelta,
	DirectX::FXMMATRIX matVP)
{
	if (nullptr == entity)
	{
		selX = false;
		selY = false;
		selZ = false;
		return;
	}

	InputManager& input = InputManager::instance();

	auto t = entity->GetComponent<Transform>();

	XMMATRIX worldMat = XMMatrixTranspose(XMLoadFloat4x4(t->GetWorldMatrix()));
	XMVECTOR worldRot = XMQuaternionRotationMatrix(worldMat);
	
	XMVECTOR worldPos = XMLoadFloat3(t->GetWorldPosition());
	XMVECTOR worldXDir = XMVector3Normalize(XMVector3Rotate(XMVectorSet(1, 0, 0, 0), worldRot));
	XMVECTOR worldYDir = XMVector3Normalize(XMVector3Rotate(XMVectorSet(0, 1, 0, 0), worldRot));
	XMVECTOR worldZDir = XMVector3Cross(worldXDir, worldYDir);

	if (input.GetLeftMouseHeld())
	{
		if (selX || selY || selZ)
		{
			XMVECTOR screenPos = XMVector3TransformCoord(worldPos, matVP);
			XMVECTOR screenXDir = XMVector3Normalize(
				XMVector3TransformCoord(worldPos + worldXDir, matVP) - screenPos
			);
			XMVECTOR screenYDir = XMVector3Normalize(
				XMVector3TransformCoord(worldPos + worldYDir, matVP) - screenPos
			);
			XMVECTOR screenZDir = XMVector3Normalize(
				XMVector3TransformCoord(worldPos + worldZDir, matVP) - screenPos
			);

			if (mode == Mode::Translate)
			{
				UpdateTranslator(entity,
					XMVectorGetX(XMVector3Dot(mouseWorldDelta, screenXDir)),
					XMVectorGetX(XMVector3Dot(mouseWorldDelta, screenYDir)),
					XMVectorGetX(XMVector3Dot(mouseWorldDelta, screenZDir)));
			}
			else if (mode == Mode::Rotate)
			{
				UpdateRotator(entity,
					XMVectorGetX(XMVector3Dot(mouseWorldDelta, screenXDir)),
					XMVectorGetX(XMVector3Dot(mouseWorldDelta, screenYDir)),
					XMVectorGetX(XMVector3Dot(mouseWorldDelta, screenZDir)));
			}
			else if (mode == Mode::Scale)
			{
				UpdateScalor(entity,
					XMVectorGetX(XMVector3Dot(mouseWorldDelta, screenXDir)),
					XMVectorGetX(XMVector3Dot(mouseWorldDelta, screenYDir)),
					XMVectorGetX(XMVector3Dot(mouseWorldDelta, screenZDir)));
			}
		}
	}
	else
	{
		XMVECTOR planeXY = XMPlaneFromPointNormal(worldPos, worldZDir);
		XMVECTOR planeXZ = XMPlaneFromPointNormal(worldPos, worldYDir);
		XMVECTOR planeYZ = XMPlaneFromPointNormal(worldPos, worldXDir);

		XMStoreFloat3(&hitXY, XMPlaneIntersectLine(planeXY, rayStart, rayStart + rayDir * 100.0f) - worldPos);
		XMStoreFloat3(&hitXZ, XMPlaneIntersectLine(planeXZ, rayStart, rayStart + rayDir * 100.0f) - worldPos);
		XMStoreFloat3(&hitYZ, XMPlaneIntersectLine(planeYZ, rayStart, rayStart + rayDir * 100.0f) - worldPos);

		if (mode == Mode::Translate)
		{
			UpdateTranslator();
		}
		else if (mode == Mode::Rotate)
		{
			UpdateRotator();
		}
		else if (mode == Mode::Scale)
		{
			UpdateScalor();
		}
	}

	if (mode == Mode::Translate)
	{
		DrawTranslator(worldPos, worldXDir, worldYDir, worldZDir);
	}
	else if (mode == Mode::Rotate)
	{
		DrawRotator(worldPos, worldXDir, worldYDir, worldZDir);
	}
	else if (mode == Mode::Scale)
	{
		DrawScalor(worldPos, worldXDir, worldYDir, worldZDir);
	}
}

void HandleGizmo::UpdateTranslator()
{
	selX = 
		(hitXY.x > 0.1f && hitXY.x <= 1.0f) && (hitXY.y > -0.1f && hitXY.y < 0.1f) ||
		(hitXZ.x > 0.1f && hitXZ.x <= 1.0f) && (hitXZ.z > -0.1f && hitXZ.z < 0.1f);
	selY = 
		(hitXY.y > 0.1f && hitXY.y <= 1.0f) && (hitXY.x > -0.1f && hitXY.x < 0.1f) ||
		(hitYZ.y > 0.1f && hitYZ.y <= 1.0f) && (hitYZ.z > -0.1f && hitYZ.z < 0.1f);
	selZ = 
		(hitXZ.z > 0.1f && hitXZ.z <= 1.0f) && (hitXZ.x > -0.1f && hitXZ.x < 0.1f) ||
		(hitYZ.z > 0.1f && hitYZ.z <= 1.0f) && (hitYZ.y > -0.1f && hitYZ.y < 0.1f);

	if (selX)
	{
		selY = false;
		selZ = false;
	}
	else if (selY)
	{
		selZ = false;
	}
}

void HandleGizmo::UpdateTranslator(Entity* e, float deltaX, float deltaY, float deltaZ)
{
	auto t = e->GetComponent<Transform>();
	XMFLOAT3 localPos = *(t->GetLocalPosition());
	
	if (selX)
	{
		localPos.x += deltaX;
	}
	if (selY)
	{
		localPos.y += deltaY;
	}
	if (selZ)
	{
		localPos.z += deltaZ;
	}

	t->SetLocalPosition(localPos.x, localPos.y, localPos.z);
}

void _vectorcall HandleGizmo::DrawTranslator(DirectX::XMVECTOR worldPos, DirectX::XMVECTOR worldXDir, DirectX::XMVECTOR worldYDir, DirectX::XMVECTOR worldZDir)
{
	auto* gizmo = GizmoRenderer::xray();
	gizmo->DrawLine(worldPos, worldPos + worldXDir, selX ? PrimitiveRenderer::yellow : PrimitiveRenderer::red);
	gizmo->DrawLine(worldPos, worldPos + worldYDir, selY ? PrimitiveRenderer::yellow : PrimitiveRenderer::green);
	gizmo->DrawLine(worldPos, worldPos + worldZDir, selZ ? PrimitiveRenderer::yellow : PrimitiveRenderer::blue);
}

void HandleGizmo::UpdateRotator()
{
	float radiusYZ = std::sqrt(hitYZ.y * hitYZ.y + hitYZ.z * hitYZ.z);
	float radiusXZ = std::sqrt(hitXZ.x * hitXZ.x + hitXZ.z * hitXZ.z);
	float radiusXY = std::sqrt(hitXY.x * hitXY.x + hitXY.y * hitXY.y);
	selX = radiusYZ > 0.9f && radiusYZ < 1.1f;
	selY = radiusXZ > 0.9f && radiusXZ < 1.1f;
	selZ = radiusXY > 0.9f && radiusXY < 1.1f;

	if (selX)
	{
		selY = false;
		selZ = false;
	}
	else if (selY)
	{
		selZ = false;
	}
}

void HandleGizmo::UpdateRotator(Entity * e, float deltaX, float deltaY, float deltaZ)
{
}

void _vectorcall HandleGizmo::DrawRotator(DirectX::XMVECTOR worldPos, DirectX::XMVECTOR worldXDir, DirectX::XMVECTOR worldYDir, DirectX::XMVECTOR worldZDir)
{
	auto* gizmo = GizmoRenderer::xray();
	gizmo->DrawRing(worldPos, worldYDir, worldZDir, selX ? PrimitiveRenderer::yellow : PrimitiveRenderer::red);
	gizmo->DrawRing(worldPos, worldXDir, worldZDir, selY ? PrimitiveRenderer::yellow : PrimitiveRenderer::green);
	gizmo->DrawRing(worldPos, worldXDir, worldYDir, selZ ? PrimitiveRenderer::yellow : PrimitiveRenderer::blue);
}

void HandleGizmo::UpdateScalor()
{
	selX =
		(hitXY.x > 0.1f && hitXY.x <= 1.0f) && (hitXY.y > -0.1f && hitXY.y < 0.1f) ||
		(hitXZ.x > 0.1f && hitXZ.x <= 1.0f) && (hitXZ.z > -0.1f && hitXZ.z < 0.1f);
	selY =
		(hitXY.y > 0.1f && hitXY.y <= 1.0f) && (hitXY.x > -0.1f && hitXY.x < 0.1f) ||
		(hitYZ.y > 0.1f && hitYZ.y <= 1.0f) && (hitYZ.z > -0.1f && hitYZ.z < 0.1f);
	selZ =
		(hitXZ.z > 0.1f && hitXZ.z <= 1.0f) && (hitXZ.x > -0.1f && hitXZ.x < 0.1f) ||
		(hitYZ.z > 0.1f && hitYZ.z <= 1.0f) && (hitYZ.y > -0.1f && hitYZ.y < 0.1f);

	if (selX)
	{
		selY = false;
		selZ = false;
	}
	else if (selY)
	{
		selZ = false;
	}
}

void HandleGizmo::UpdateScalor(Entity * e, float deltaX, float deltaY, float deltaZ)
{
	auto t = e->GetComponent<Transform>();
	XMFLOAT3 localScale = *(t->GetLocalScale());

	if (selX)
	{
		localScale.x += deltaX;
	}
	if (selY)
	{
		localScale.y += deltaY;
	}
	if (selZ)
	{
		localScale.z += deltaZ;
	}

	t->SetScale(localScale.x, localScale.y, localScale.z);
}

void _vectorcall HandleGizmo::DrawScalor(DirectX::XMVECTOR worldPos, DirectX::XMVECTOR worldXDir, DirectX::XMVECTOR worldYDir, DirectX::XMVECTOR worldZDir)
{
	auto* gizmo = GizmoRenderer::xray();
	gizmo->DrawLine(worldPos, worldPos + worldXDir, selX ? PrimitiveRenderer::yellow : PrimitiveRenderer::red);
	gizmo->DrawLine(worldPos, worldPos + worldYDir, selY ? PrimitiveRenderer::yellow : PrimitiveRenderer::green);
	gizmo->DrawLine(worldPos, worldPos + worldZDir, selZ ? PrimitiveRenderer::yellow : PrimitiveRenderer::blue);
}
