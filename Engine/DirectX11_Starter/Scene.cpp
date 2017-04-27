#include "Scene.h"

#include "RenderingSystem.h"
#include "Entity.h"
#include "ComponentPool.h"
#include "Lights.h"

void Scene::Enter()
{

	// Testing hard code 
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

	// Main entity 
	Entity* enti= new Entity();
	Transform* t = enti->AddComponent<Transform>();
	Renderable* r = enti->AddComponent<Renderable>(mesh, mat);
	//RigidBody* rigid = enti->AddComponent<RigidBody>(t, r->BoundingBox()); 
	AddEntity(enti);

	// Collision check entity 
	Entity* e1 = new Entity(); 
	Transform* t1 = e1->AddComponent<Transform>(); 
	t1->SetWorldPosition(10, 0, 0); 
	Renderable* r1 = e1->AddComponent<Renderable>(mesh, mat);
	//RigidBody* rb1 = e1->AddComponent <RigidBody>(t1, r1->BoundingBox());
	//e1->AddComponent<ScriptComponent>("script2.lua", rb1);
	AddEntity(e1);

	// Child entity 
	Entity* e = new Entity();
	Transform* t2 = e->AddComponent<Transform>();
	e->AddComponent<Renderable>(mesh, mat);
	AddEntity(e);


	t2->SetParent(t);
	t2->SetLocalPosition(2.0f, 0.0f, 0.0f);

	lights = new Entity();
	Lighting* l = lights->AddComponent<Lighting>(XMFLOAT4(-5, 0, 0, 0), XMFLOAT4(0.7f, 0, 0, 1), LightType::PointLight, 1, 8);
	Lighting* l2 = lights->AddComponent<Lighting>(XMFLOAT4(0, -1, 0, 0), XMFLOAT4(0, 0.5f, 0.5f, 1), LightType::DirectionalLight, 1);

	//Script 
	//RigidBody* r = new enti->AddComponent<RigidBody>(t, r->BoundingBox);
	//enti->AddComponent<ScriptComponent>("script2.lua", ); 

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

	auto* t = entities[0]->GetComponent<Transform>();
	t->SetRotationEuler(0, rot, 0);

	t = t->children[0];
	t->SetRotationEuler(rot * 10.0f, 0,  0);

	//enti->GetComponent<ScriptComponent>()->Update(); 
}

void Scene::Exit()
{
	for (Entity* e : entities) {
		delete e;
	}
	entities.clear();
	delete componentManager;
	delete lights;
	//componentManager->Release();
}

vector<Entity*> Scene::GetAllEntities()
{
	return entities;
}

void Scene::AddEntity(Entity * entity)
{
	entities.push_back(entity);
}

Entity * Scene::GetEntityByName(string name)
{
	for (Entity* e : entities) {
		if (e->GetName() == name) {
			return e;
		}
	}
	return nullptr;
}

Entity * Scene::GetEntityByID(int id)
{
	for (Entity* e : entities) {
		if (e->GetID() == id) {
			return e;
		}
	}
	return nullptr;
}
