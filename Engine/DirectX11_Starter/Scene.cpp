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

Scene::Scene()
{
	componentManager = new ComponentManager();
	ComponentManager::current = componentManager;
}

void Scene::Enter()
{
	// Testing hard code 

	RenderingSystem& renderer = *RenderingSystem::instance();

	//Mesh* mesh = renderer.CreateMesh("Assets/Models/cube.fbx");

	//Shader* shader = renderer.CreateShader(L"Assets/ShaderObjs/Opaque.cso");
	//Texture* tex = renderer.CreateTexture(L"Assets/Textures/rust.jpg");
	//Texture* tex1 = renderer.CreateTexture(L"Assets/Textures/water.jpg");

	//mat = renderer.CreateMaterial(shader);
	//mat->SetTexture("texDiffuse", tex);

	//DirectX::XMFLOAT4X4 matrix;
	//DirectX::XMStoreFloat4x4(&matrix, DirectX::XMMatrixIdentity());
	Mesh* mesh = AddMesh("cube", "Assets/Models/cube.fbx");
	mat = AddMaterial("mat1", L"Assets/ShaderObjs/Opaque.cso", "texDiffuse", L"Assets/Textures/rust.jpg");
	// Main entity 
	Entity* main = new Entity();
	Transform* mainT = main->GetComponent<Transform>();
	Renderable* mainR = main->AddComponent<Renderable>(mesh, mat);
	RigidBody* mainRb = main->AddComponent<RigidBody>(mainT, &(mainR->BoundingBox()));
	AddEntity(main);

	mat = AddMaterial("mat2", L"Assets/ShaderObjs/Opaque.cso", "texDiffuse", L"Assets/Textures/water.jpg");

	// Collision check entity 
	Entity* e1 = new Entity();

	Transform* t1 = e1->GetComponent<Transform>();
	t1->SetLocalPosition(2, 0, 0);
	Renderable* r1 = e1->AddComponent<Renderable>(mesh, mat);
	RigidBody* rb1 = e1->AddComponent <RigidBody>(t1, &(r1->BoundingBox()));
	e1->AddComponent<ScriptComponent>("script2.lua", rb1);

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

	ParticleEmitter* emitter = emitEntity->AddComponent<ParticleEmitter>(L"Assets/Textures/smoke.png");
	AddEntity(emitEntity);
	Transform* emitEntityTrans = emitEntity->GetComponent<Transform>();
	emitEntityTrans->SetLocalPosition(2.0f, 0.0f, 0.0f);

	emitter->SetInitialVelocity(0.0f, 0.3f, 0.0f);
	emitter->SetLifeTime(5.0f);
	emitter->SetEmitRate(5);

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

	t->SetWorldPosition(t->GetWorldPosition()->x + 0.5f*deltaTime, 0, 0);
	t1->SetRotationEuler(0, rot, 0);

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

Entity * Scene::CreateEntity(string name)
{
	Entity* ent = new Entity(name);
	AddEntity(ent);
	return ent;
}

Entity * Scene::CreateEntity(string name, Mesh * mesh, Material * material)
{
	Entity* ent = new Entity(name, material, mesh);
	AddEntity(ent);
	return nullptr;
}

bool Scene::DeleteEntityByName(string name)
{
	for (size_t i = 0; i < entities.size(); i++) {
		if (entities[i]->GetName() == name) {
			entities.erase(entities.begin() + i);
			return true;
		}
	}
	return false;
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

Mesh * Scene::AddMesh(string meshName, string file)
{
	RenderingSystem& renderer = *RenderingSystem::instance();
	Mesh* mesh = renderer.CreateMesh(file.c_str());
	if (mesh != nullptr) {
		meshes.push_back(std::make_pair(meshName, mesh));
	}	
	return mesh;
}

Mesh * Scene::GetMesh(string name)
{
	for (auto i : meshes) {
		if (i.first == name) {
			return i.second;
		}
	}
	return nullptr;
}

Material * Scene::AddMaterial(string materialName, const wchar_t * shaderFile, string textureName, const wchar_t * textureFile)
{
	RenderingSystem& renderer = *RenderingSystem::instance();
	Shader* shader = renderer.CreateShader(shaderFile);
	Texture* texture = renderer.CreateTexture(textureFile);
	Material* material = renderer.CreateMaterial(shader);
	material->SetTexture(materialName, texture);
	materials.push_back(std::make_pair(materialName, material));
	return material;
}

Material * Scene::GetMaterial(string name)
{
	return nullptr;
}
