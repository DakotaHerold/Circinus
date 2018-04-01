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
#include "BoundRenderer.h"

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

	// Main entity 
	Entity* main = new Entity();
	Transform* mainT = main->AddComponent<Transform>();
	Renderable* mainR = main->AddComponent<Renderable>(mesh, mat);
	RigidBody* mainRb = main->AddComponent<RigidBody>(mainT, &(mainR->BoundingBox()));
	AddEntity(main);

	mat = renderer.CreateMaterial(shader);
	mat->SetTexture("texDiffuse", tex1);

	// Collision check entity 
	Entity* e1 = new Entity();
	Transform* t1 = e1->AddComponent<Transform>();

	t1->SetLocalPosition(2, 0, 0);
	Renderable* r1 = e1->AddComponent<Renderable>(mesh, mat);
	RigidBody* rb1 = e1->AddComponent <RigidBody>(t1, &(r1->BoundingBox()));
	//e1->AddComponent<ScriptComponent>("script2.lua", rb1);

	//
	t1->SetParent(mainT);
	//
	AddEntity(e1);

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


	// OctTree Initialization --------------------------------------
	//Point* point = new Point(0, 0, 17.5f, 1);
	pointCount = 7;
	points = new Point*[pointCount]; 
	points[0] = new Point(-1.0f, 0.0f, 20.0f, 0);
	points[1] = new Point(0.0f, 0.0f, 15.0f, 1);
	points[2] = new Point(2.0f, 0.0f, 23.0f, 2);
	points[3] = new Point(3.0f, -4.0f, 15.0f, 3);
	points[4] = new Point(3.0f, 3.9f, 15.0f, 4);
	points[5] = new Point(3.0f, 3.5f, 15.0f, 5);
	points[6] = new Point(0.0f, 0.0f, 20.0f, 4);



	DirectX::BoundingBox aabb(
		DirectX::XMFLOAT3(0, 0, 20),
		DirectX::XMFLOAT3(5, 5, 5));
	DirectX::BoundingOrientedBox::CreateFromBoundingBox(box, aabb);
	box.Transform(box, 1,
		DirectX::XMQuaternionRotationRollPitchYaw(0, 0, 0)
		, DirectX::XMVectorSet(0, 0, 0, 0));
	
	oct = new Octree(); 
	oct->build(points, pointCount, 1, 10, box, 0);

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
	auto* t1 = entities[1]->GetComponent<Transform>();

	//t->SetWorldPosition(t->GetWorldPosition()->x + 0.5f*deltaTime, 0, 0);
	t1->SetRotationEuler(0, rot, 0);

	auto* r = entities[0]->GetComponent<RigidBody>();
	auto* r1 = entities[1]->GetComponent<RigidBody>();
	//cout << r->BoxCollisionCheck(r1) << endl;


	//enti->GetComponent<ScriptComponent>()->Update(); 

	// BOX DEBUG RENDERING
	//{
	//	DirectX::BoundingSphere sphere(DirectX::XMFLOAT3(0, 0, 0), 2);
	//	BoundRenderer::instance()->Draw(sphere);

	//	DirectX::BoundingBox aabb(
	//		DirectX::XMFLOAT3(0, 0, 0),
	//		DirectX::XMFLOAT3(1, 1, 1));
	//	DirectX::BoundingOrientedBox box;
	//	DirectX::BoundingOrientedBox::CreateFromBoundingBox(box, aabb);
	//	box.Transform(box, 1,
	//		DirectX::XMQuaternionRotationRollPitchYaw(0, rot, 0)
	//		, DirectX::XMVectorSet(0, 0, 0, 0));
	//	BoundRenderer::instance()->Draw(box);
	//}
	//box.Extents.x += 0.001f;
	//BoundRenderer::instance()->Draw(box);
	//points[0]->y += 0.0001f; 
	//points[0]->x += 0.0001f;

	if (points[0] != nullptr)
	{
		if (points[0]->enabled == false)
		{
			points[0] = nullptr;
		}
		else
		{
			points[0]->Move(0, 0, -0.001f);
		}
		
	}
	
	//oct->build(points, pointCount, 1, 10, box, 0);
	oct->Update(); 
}

void Scene::Exit()
{
	for (Entity* e : entities) {
		delete e;
	}
	entities.clear();

	delete lights;

	delete oct;

	for (int i = 0; i < pointCount; i++)
	{
		delete[] points[i];
	}
	delete[] points;


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
