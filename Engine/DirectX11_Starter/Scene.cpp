#include "Scene.h"

#include "RenderingSystem.h"
#include "Entity.h"
#include "ComponentPool.h"

void Scene::Enter()
{
	componentManager = new ComponentManager();
	ComponentManager::current = componentManager;

	RenderingSystem& renderer = *RenderingSystem::instance();

	Mesh* mesh = renderer.CreateMesh("Assets/Models/cube.fbx");

	Shader* shader = renderer.CreateShader(L"Assets/ShaderObjs/Opaque.cso");
	Texture* tex = renderer.CreateTexture(L"Assets/Textures/rust.jpg");

	mat = renderer.CreateMaterial(shader);
	mat->SetTexture("texDiffuse", tex);

	DirectX::XMFLOAT4X4 matrix;
	DirectX::XMStoreFloat4x4(&matrix, DirectX::XMMatrixIdentity());

	enti = new Entity();
	Transform* t = enti->AddComponent<Transform>();
	Renderable* r = enti->AddComponent<Renderable>(mesh, mat);

	e = new Entity();
	Transform* t2 = e->AddComponent<Transform>();
	e->AddComponent<Renderable>(mesh, mat);

	t2->SetParent(t);
	t2->SetPosition(2.0f, 0.0f, 0.0f);

	//Script 
	//enti->AddComponent<ScriptComponent>("script2.lua", t); 

	// skybox

	Shader* skyboxShader = renderer.CreateShader(L"Assets/ShaderObjs/Skybox.cso");
	Texture* skyboxTex = renderer.CreateTexture(L"Assets/Textures/space.dds");
	Material* skyboxMaterial = renderer.CreateMaterial(skyboxShader);

	skyboxMaterial->SetTexture("skyMap", skyboxTex);

	Renderable* skybox = sceneGraph.CreateRenderable();
	skybox->SetMesh(mesh);
	skybox->SetMaterial(skyboxMaterial);

	sceneGraph.SetSkyBox(skybox);

	cam.getViewMatrix();
	cam.setProjectionMatrix(800.0f / 600.0f);

	//component manager

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

	auto* t = enti->GetComponent<Transform>();
	t->SetRotationEuler(0, rot, 0);

	t = t->children[0];
	t->SetRotationEuler(rot * 10.0f, 0,  0);

	//enti->GetComponent<ScriptComponent>()->Update(); 
}

void Scene::Exit()
{
	delete enti;
	delete e;
	delete componentManager;
	//componentManager->Release();
}
