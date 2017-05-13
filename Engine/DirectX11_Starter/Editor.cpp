#include "Editor.h"

#ifdef EDITOR_BUILD

#include "RenderingSystem.h"
#include "InputManager.h"
#include "ComponentManager.h"
#include "Transform.h"
#include "Renderable.h"
#include "GizmoRenderer.h"

Editor* Editor::_instance = nullptr;

void Editor::Init()
{
	assert(nullptr == _instance);
	_instance = this;

	cam.getViewMatrix();
	cam.setProjectionMatrix(800.0f / 600.0f);
}

void Editor::Update(float deltaTime, float totalTime)
{
	InputManager& input = InputManager::instance();

	// camera movement
	cam.update(deltaTime);

	if (!handle.IsMoving())
	{
		float x = input.GetMouseMoveX();
		if (input.GetMiddleMouseHeld())	// Left button is down
		{
			cam.moveSideways(input.GetMouseMoveX() *0.005f);
			cam.moveVertical(input.GetMouseMoveY() * 0.005f);
		}
		if (input.GetLeftMouseHeld())	// Right button is down
		{
			cam.setRotationY(input.GetMouseMoveX());
			cam.setRotationX(input.GetMouseMoveY());
		}

		cam.moveAlongDirection(input.GetMouseWheelDelta() * 0.01f);
	}

	{
		GizmoRenderer::opaque()->DrawGrid(
			DirectX::XMFLOAT3{ 10, 0, 0 },
			DirectX::XMFLOAT3{ 0, 0, 10 },
			DirectX::XMFLOAT3{ 0, 0, 0 },
			10, 10,
			DirectX::XMFLOAT3{ 0.3f, 0.3f, 0.3f }
		);
		GizmoRenderer::opaque()->DrawCoordinate(cam.getViewMatrix(), cam.getProjectionMatrix(), XMFLOAT3{ -0.8f, -0.8f, 0.5f }, 0.01f);
	}

	XMVECTOR mouseRayStart, mouseRayDirection, mouseWorldDelta;
	XMMATRIX matVP, matVP_Inv;

	// get mouse ray
	{
		const auto& frustum = cam.getFrustum();

		matVP = XMMatrixTranspose(XMLoadFloat4x4(&cam.getViewMatrix()));
		matVP = XMMatrixMultiply(
			matVP,
			XMMatrixTranspose(XMLoadFloat4x4(&cam.getProjectionMatrix()))
		);
		matVP_Inv = XMMatrixInverse(nullptr, matVP);

		float mouseClipSpaceX = input.GetMousePositionX() / RenderingSystem::instance()->GetWindowWidth() * 2.0f - 1.0f;
		float mouseClipSpaceY = -input.GetMousePositionY() / RenderingSystem::instance()->GetWindowHeight() * 2.0f + 1.0f;

		XMVECTOR mousePos = XMVectorSet(
			mouseClipSpaceX,
			mouseClipSpaceY,
			(frustum.Near == 0.0f ? 1.0f : frustum.Near),
			0
		);

		mousePos = XMVector3TransformCoord(mousePos, matVP_Inv)
			- XMVectorSet(0, 0, 0, 1);

		mouseRayStart = XMLoadFloat3(&cam.getPosition());
		mouseRayDirection = XMVector3Normalize(mousePos - mouseRayStart);

		mouseWorldDelta = XMVectorSet(
			input.GetMouseMoveX() * deltaTime,// / RenderingSystem::instance()->GetWindowWidth(),
			-input.GetMouseMoveY() * deltaTime,// / RenderingSystem::instance()->GetWindowHeight(),
			0,
			0
		);
	}

	// handle gizmos
	if (nullptr != selectedEntity)
	{
		auto r = selectedEntity->GetComponent<Renderable>();
		auto t = selectedEntity->GetComponent<Transform>();

		XMMATRIX worldMat = XMMatrixTranspose(
			XMLoadFloat4x4(t->GetWorldMatrix())
		);

		BoundingOrientedBox box;
		BoundingOrientedBox::CreateFromBoundingBox(box, r->GetMesh()->GetBounds());
		box.Transform(box, worldMat);

		GizmoRenderer::opaque()->Draw(box, PrimitiveRenderer::green);	
	}

	handle.Update(selectedEntity, deltaTime, totalTime, mouseRayStart, mouseRayDirection, mouseWorldDelta, matVP);

	// mouse picking
	if (input.GetLeftMouseDown() && !handle.IsMoving())
	{
		float minDist = FLT_MAX;
		Renderable* selectedObj = nullptr;

		ResultComponents<Renderable> r = ComponentManager::current->GetAllComponents<Renderable>();

		for (size_t j = 0; j < r.size; j++) {
			Renderable* i = &r.components[j];

			Transform* t = i->GetEntity()->GetComponent<Transform>();
			auto* m = t->GetWorldMatrix();

			XMMATRIX worldMat = XMMatrixTranspose(
				XMLoadFloat4x4(m)
			);

			BoundingOrientedBox bounds;
			BoundingOrientedBox::CreateFromBoundingBox(bounds, i->GetMesh()->GetBounds());
			bounds.Transform(bounds, worldMat);

			float dist = FLT_MAX;
			if (bounds.Intersects(mouseRayStart, mouseRayDirection, dist))
			{
				if (dist < minDist)
				{
					minDist = dist;
					selectedObj = i;
				}
			}
		}

		if (nullptr != selectedObj)
		{
			selectedEntity = selectedObj->GetEntity();
		}
		else
		{
			selectedEntity = nullptr;
		}
	}

}

void Editor::CleanUp()
{
	assert(nullptr != _instance);
	delete _instance;
	_instance = nullptr;
}

#endif