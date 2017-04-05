#include "Scene.h"

#include "RenderingSystem.h"

void Scene::Enter()
{
	RenderingSystem& renderer = *RenderingSystem::instance();

	Mesh* mesh = renderer.CreateMesh("Assets/Models/cube.fbx");

	Shader* shader = renderer.CreateShader(L"Assets/ShaderObjs/Opaque.cso");
	Texture* tex = renderer.CreateTexture(L"Assets/Textures/crate.png");

	mat = renderer.CreateMaterial(shader);

	mat->SetTexture("texDiffuse", tex);

	Renderable* r = sceneGraph.CreateRenderable();
	r->SetMesh(mesh);
	r->SetMaterial(mat);

	DirectX::XMFLOAT4X4 matrix;
	DirectX::XMStoreFloat4x4(&matrix, DirectX::XMMatrixIdentity());

	mat->SetMatrix4x4("matWorld", matrix);
	mat->SetMatrix4x4("matWorld_IT", matrix);

	// skybox

	Shader* skyboxShader = renderer.CreateShader(L"Assets/ShaderObjs/Skybox.cso");
	Texture* skyboxTex = renderer.CreateTexture(L"Assets/Textures/space.dds");
	Material* skyboxMaterial = renderer.CreateMaterial(skyboxShader);

	skyboxMaterial->SetTexture("skyMap", skyboxTex);

	Renderable* skybox = sceneGraph.CreateRenderable();
	skybox->SetMesh(mesh);
	skybox->SetMaterial(skyboxMaterial);

	cam.getViewMatrix();
	cam.setProjectionMatrix(800.0f / 600.0f);
}

void Scene::Tick(float deltaTime, float totalTime)
{
	static float rot = 0.0f;

	cam.update(deltaTime);
	InputManager& input = InputManager::instance();

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

	rot += deltaTime * 1.0f;

	DirectX::XMFLOAT4X4 matrix;
	auto rotM = DirectX::XMMatrixRotationY(rot);
	DirectX::XMStoreFloat4x4(&matrix, DirectX::XMMatrixTranspose(rotM));
	mat->SetMatrix4x4("matWorld", matrix);
	DirectX::XMStoreFloat4x4(&matrix, DirectX::XMMatrixInverse(nullptr, rotM));
	mat->SetMatrix4x4("matWorld_IT", matrix);


}

void Scene::Exit()
{
}
