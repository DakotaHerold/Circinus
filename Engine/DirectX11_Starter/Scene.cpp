#include "Scene.h"
#include "RenderingSystem.h"
#include "Entity.h"
#include "Lights.h"
#include "Lighting.h"
#include "ClassTypeId.h"
#include <iostream>
#include <string.h>
#include "RigidBody.h"
#include "ScriptComponent.h"
#include "ParticleEmitter.h"
#include "GizmoRenderer.h"

void Scene::Enter()
{
	// Testing hard code 
	componentManager = new ComponentManager();
	ComponentManager::current = componentManager;

	RenderingSystem& renderer = *RenderingSystem::instance();

	Mesh* mesh = renderer.CreateMesh("Assets/Models/cube.fbx");

	Shader* shader = renderer.CreateShader(L"Assets/ShaderObjs/Opaque.cso");
	Texture* tex = renderer.CreateTexture(L"Assets/Textures/rust.jpg");
	Texture* tex1 = renderer.CreateTexture(L"Assets/Textures/water.jpg");

	mat = renderer.CreateMaterial(shader);
	mat->SetTexture("texDiffuse", tex);

	DirectX::XMFLOAT4X4 matrix;
	DirectX::XMStoreFloat4x4(&matrix, DirectX::XMMatrixIdentity());

	//// Main entity 
	//Entity* main = new Entity();
	//Transform* mainT = main->AddComponent<Transform>();
	//Renderable* mainR = main->AddComponent<Renderable>(mesh, mat);
	//RigidBody* mainRb = main->AddComponent<RigidBody>(mainT, &(mainR->BoundingBox()));
	//AddEntity(main);

	//mat = renderer.CreateMaterial(shader);
	//mat->SetTexture("texDiffuse", tex1);

	//// Collision check entity 
	//Entity* e1 = new Entity();
	//Transform* t1 = e1->AddComponent<Transform>();

	//t1->SetLocalPosition(2, 0, 0);
	//Renderable* r1 = e1->AddComponent<Renderable>(mesh, mat);
	//RigidBody* rb1 = e1->AddComponent <RigidBody>(t1, &(r1->BoundingBox()));
	//e1->AddComponent<ScriptComponent>("script2.lua", rb1);

	////
	//t1->SetParent(mainT);
	////
	//AddEntity(e1);

	// Test FaceTo entity
	Entity* facing = new Entity();
	Transform* facingT = facing->AddComponent<Transform>();
	Renderable* facingR = facing->AddComponent<Renderable>(mesh, mat);
	RigidBody* facingRb = facing->AddComponent<RigidBody>(facingT, &(facingR->BoundingBox()));
	AddEntity(facing);

	mat = renderer.CreateMaterial(shader);
	mat->SetTexture("texDiffuse", tex1);

	Entity* target = new Entity();
	Transform* targetT = target->AddComponent<Transform>();

	targetT->SetLocalPosition(2, 0, 0);
	Renderable* targetR = target->AddComponent<Renderable>(mesh, mat);
	RigidBody* targetRb = target->AddComponent <RigidBody>(targetT, &(targetR->BoundingBox()));
	//target->AddComponent<ScriptComponent>("script2.lua", rb1);

	AddEntity(target);

	Entity* facingC = new Entity();
	Transform* facingCT = facingC->AddComponent<Transform>();
	Renderable* facingCR = facingC->AddComponent<Renderable>(mesh, mat);
	RigidBody* facingCRb = facingC->AddComponent<RigidBody>(facingCT, &(facingCR->BoundingBox()));

	facingCT->SetParent(facingT);
	facingCT->SetScale(0.3, 0.3, 0.3);
	facingCT->SetLocalPosition(0, 0, 1);
	AddEntity(facingC);

	mat = renderer.CreateMaterial(shader);
	mat->SetTexture("texDiffuse", tex1);


	////Child entity 
	//Entity* e2 = new Entity();
	//Transform* t2 = e2->AddComponent<Transform>();
	//Renderable* r2 = e2->AddComponent<Renderable>(mesh, mat);
	//RigidBody* rb2 = e2->AddComponent<RigidBody>(t2, &(r2->BoundingBox()));
	//AddEntity(e2);

	//t2->SetParent(t1);
	//t2->SetLocalPosition(1.0f, 0.0f, 0.0f);

	lights = new Entity();
	Lighting* l = lights->AddComponent<Lighting>(XMFLOAT4(-5, 0, 0, 0), XMFLOAT4(0.7f, 0, 0, 1), LightType::PointLight, 1, 8);
	Lighting* l2 = lights->AddComponent<Lighting>(XMFLOAT4(0, -1, 0, 0), XMFLOAT4(0, 0.5f, 0.5f, 1), LightType::DirectionalLight, 1);

	// Particle Emitters;
	Entity* emitEntity = new Entity();
	Transform* emitEntityTrans = emitEntity->AddComponent<Transform>();
	ParticleEmitter* emitter = emitEntity->AddComponent<ParticleEmitter>(L"Assets/Textures/smoke.png");
	AddEntity(emitEntity);

	emitEntityTrans->SetLocalPosition(2.0f, 0.0f, 0.0f);

	emitter->SetInitialVelocity(0.0f, 0.3f, 0.0f);
	emitter->SetLifeTime(5.0f);
	emitter->SetEmitRate(5);

	// Sravan
	// Get all components of an entity with id 
	/*for (auto i : componentManager->GetAllComponents(lights->GetID())) {
		// get component name by component type
		std::cout << ComponentTypeName(i.first) << std::endl;

		// if you need specific data of the component, you need to manually add those kind of shitty code because c++ has no reflection
		// maybe there are some good solutions I don't know
		if (strcmp(ComponentTypeName(i.first), "class Lighting") == 0) {
			cout << "Find Lighting!" << std::endl;;
			Lighting* t = componentManager->GetComponent<Lighting>(lights->GetID(), *i.second);
		}
	}*/

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
	auto* tR = entities[0]->GetComponent<RigidBody>();
	auto* t1 = entities[1]->GetComponent<Transform>();
	auto* t1R = entities[1]->GetComponent<RigidBody>();

	static int moveDir = 1;

	if (t1->GetWorldPosition()->y < -2)
	{
		moveDir = 1;
	}

	else if (t1->GetWorldPosition()->y > 2)
	{
		moveDir = -1;
	}

	t1->SetWorldPosition(t1->GetWorldPosition()->x, t1->GetWorldPosition()->y + 0.5f*deltaTime * moveDir, 0);

	tR->FaceTo(t1R);
	//t1->SetRotationEuler(0, rot, 0);

	auto* r = entities[0]->GetComponent<RigidBody>();
	auto* r1 = entities[1]->GetComponent<RigidBody>();
	//cout << r->CollisionCheck(r1) << endl;


	//enti->GetComponent<ScriptComponent>()->Update(); 

	/*{
		DirectX::BoundingSphere sphere(DirectX::XMFLOAT3(0, 0, 0), 2);
		GizmoRenderer::instance()->Draw(sphere);

		DirectX::BoundingBox aabb(
			DirectX::XMFLOAT3(0, 0, 0),
			DirectX::XMFLOAT3(1, 1, 1));
		DirectX::BoundingOrientedBox box;
		DirectX::BoundingOrientedBox::CreateFromBoundingBox(box, aabb);
		box.Transform(box, 1,
			DirectX::XMQuaternionRotationRollPitchYaw(0, rot, 0)
			, DirectX::XMVectorSet(0, 0, 0, 0));
		GizmoRenderer::instance()->Draw(box, GizmoRenderer::red);
	}*/
}

void Scene::Exit()
{
	for (Entity* e : entities) {
		delete e;
	}
	entities.clear();

	delete lights;

	// all entities should be deleted before deleting component manager
	delete componentManager;
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
