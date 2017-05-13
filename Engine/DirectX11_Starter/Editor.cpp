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

	{
		float x = input.GetMouseMoveX();
		if (input.GetLeftMouseHeld())	// Left button is down
		{
			cam.moveSideways(input.GetMouseMoveX() *0.005f);
			cam.moveVertical(input.GetMouseMoveY() * 0.005f);
		}
		if (input.GetRightMouseHeld())	// Right button is down
		{
			cam.setRotationY(input.GetMouseMoveX());
			cam.setRotationX(input.GetMouseMoveY());
		}

		cam.moveAlongDirection(input.GetMouseWheelDelta() * 0.01f);
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

		GizmoRenderer::instance()->Draw(box, GizmoRenderer::green);
	}

	// mouse picking
	if (input.GetLeftMouseDown())
	{
		const auto& frustum = cam.getFrustum();

		XMMATRIX matVP_Inv = XMMatrixTranspose(XMLoadFloat4x4(&cam.getViewMatrix()));
		matVP_Inv = XMMatrixMultiply(
			matVP_Inv,
			XMMatrixTranspose(XMLoadFloat4x4(&cam.getProjectionMatrix()))
		);
		matVP_Inv = XMMatrixInverse(nullptr, matVP_Inv);

		ResultComponents<Renderable> r = ComponentManager::current->GetAllComponents<Renderable>();

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

		XMVECTOR cameraPos = XMLoadFloat3(&cam.getPosition());
		XMVECTOR dir = XMVector3Normalize(mousePos - cameraPos);

		float minDist = FLT_MAX;
		Renderable* selectedObj = nullptr;

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
			if (bounds.Intersects(cameraPos, dir, dist))
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