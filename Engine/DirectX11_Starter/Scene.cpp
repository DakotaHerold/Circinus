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

namespace {
	struct Str
	{
		Str(const wchar_t* str) :ws(str),s(ws.begin(),ws.end()){}
		wstring ws;
		string s;
		const char* operator() ()const {
			return s.c_str();
		}
	};
	struct Wstr
	{
		Wstr(const char* str) :s(str), ws(s.begin(), s.end()) {}
		string s;
		wstring ws;		
		const wchar_t* operator() ()const {
			return ws.c_str();
		}
	};
}

Scene::Scene()
{
	componentManager = new ComponentManager();
	ComponentManager::current = componentManager;
	root = new Transform();
	
}

void Scene::Enter()
{

	RenderingSystem& renderer = *RenderingSystem::instance();

	Shader* s1 = AddShader("Opaque", L"Assets/ShaderObjs/Opaque.cso");

	Shader* skyboxShader = AddShader("skybox", L"Assets/ShaderObjs/Skybox.cso");

	Texture* tex = AddTexture("texDiffuse", "rust", L"Assets/Textures/rust.jpg");

	Texture* tex1 = AddTexture("texDiffuse", "water", L"Assets/Textures/water.gif");

	Texture* skyboxTex = AddTexture("skyMap", "skybox", L"Assets/Textures/space.dds");

	Mesh* mesh = AddMesh("cube", L"Assets/Models/cube.fbx");

	Material* skyboxMat = AddMaterial("skybox", "skybox", "skybox");

	Material* rust= AddMaterial("rust", "Opaque", "rust");

	Material* water= AddMaterial("water", "Opaque", "water");

	Renderable* skybox = sceneGraph.CreateRenderable();

	skyboxMaterial = "skybox";

	skyboxMesh = "cube";

	skybox->SetMaterial(GetMaterial("skybox").pointer);

	skybox->SetMesh(GetMesh("cube").pointer);

	sceneGraph.SetSkyBox(skybox);

	//// Main entity 
	//Entity* main = CreateEntity("parent","cube", "rust");

	//Transform* t2 = main->GetComponent<Transform>();
	////RigidBody* mainRb = main->AddComponent<RigidBody>(main->GetComponent<Transform>(), &(main->GetComponent<Renderable>()->BoundingBox()));

	//// Collision check entity 
	//Entity* e1 = CreateEntity("child", "cube", "water");
	//Transform* t = e1->GetComponent<Transform>();
	//t->SetLocalPosition(2, 0, 0);
	//t->SetParent(main->GetComponent<Transform>());

	// Test FaceTo entity
	Entity* facing1 = CreateEntity("facing1", "cube", "rust");
	Transform* facing1T = facing1->GetComponent<Transform>();
	facing1T->SetLocalPosition(-8, 0, 0);
	Renderable* facing1R = facing1->GetComponent<Renderable>();
	RigidBody* facing1Rb = facing1->AddComponent<RigidBody>(facing1T, &(facing1R->BoundingBox()));
	facing1T->SetScale(0.4, 0.4, 0.4); 
	
	Entity* facing1C = CreateEntity("facing1C", "cube", "rust");
	Transform* facing1CT = facing1C->GetComponent<Transform>();
	Renderable* facing1CR = facing1C->GetComponent<Renderable>();
	RigidBody* facing1CRb = facing1C->AddComponent<RigidBody>(facing1CT, &(facing1CR->BoundingBox()));
	facing1CT->SetParent(facing1T);
	facing1CT->SetScale(0.8, 0.8, 0.8);
	facing1CT->SetLocalPosition(0, 0, 0.6);

	Entity* facing2 = CreateEntity("facing2", "cube", "rust");
	Transform* facing2T = facing2->GetComponent<Transform>();
	facing2T->SetLocalPosition(-8, 1, 0);
	Renderable* facing2R = facing2->GetComponent<Renderable>();
	RigidBody* facing2Rb = facing2->AddComponent<RigidBody>(facing2T, &(facing2R->BoundingBox()));
	facing2T->SetScale(0.4, 0.4, 0.4);

	Entity* facing2C = CreateEntity("facing2C", "cube", "rust");
	Transform* facing2CT = facing2C->GetComponent<Transform>();
	Renderable* facing2CR = facing2C->GetComponent<Renderable>();
	RigidBody* facing2CRb = facing2C->AddComponent<RigidBody>(facing2CT, &(facing2CR->BoundingBox()));
	facing2CT->SetParent(facing2T);
	facing2CT->SetScale(0.8, 0.8, 0.8);
	facing2CT->SetLocalPosition(0, 0, 0.6);

	Entity* facing3 = CreateEntity("facing3", "cube", "rust");
	Transform* facing3T = facing3->GetComponent<Transform>();
	facing3T->SetLocalPosition(-8, -1, 0);
	Renderable* facing3R = facing3->GetComponent<Renderable>();
	RigidBody* facing3Rb = facing3->AddComponent<RigidBody>(facing3T, &(facing3R->BoundingBox()));
	facing3T->SetScale(0.4, 0.4, 0.4);

	Entity* facing3C = CreateEntity("facing3C", "cube", "rust");
	Transform* facing3CT = facing3C->GetComponent<Transform>();
	Renderable* facing3CR = facing3C->GetComponent<Renderable>();
	RigidBody* facing3CRb = facing3C->AddComponent<RigidBody>(facing3CT, &(facing3CR->BoundingBox()));
	facing3CT->SetParent(facing3T);
	facing3CT->SetScale(0.8, 0.8, 0.8);
	facing3CT->SetLocalPosition(0, 0, 0.6);

	Entity* facing4 = CreateEntity("facing4", "cube", "rust");
	Transform* facing4T = facing4->GetComponent<Transform>();
	facing4T->SetLocalPosition(-8, 0, 1);
	Renderable* facing4R = facing4->GetComponent<Renderable>();
	RigidBody* facing4Rb = facing4->AddComponent<RigidBody>(facing4T, &(facing4R->BoundingBox()));
	facing4T->SetScale(0.4, 0.4, 0.4);

	Entity* facing4C = CreateEntity("facing4C", "cube", "rust");
	Transform* facing4CT = facing4C->GetComponent<Transform>();
	Renderable* facing4CR = facing4C->GetComponent<Renderable>();
	RigidBody* facing4CRb = facing4C->AddComponent<RigidBody>(facing4CT, &(facing4CR->BoundingBox()));
	facing4CT->SetParent(facing4T);
	facing4CT->SetScale(0.8, 0.8, 0.8);
	facing4CT->SetLocalPosition(0, 0, 0.6);

	Entity* facing5 = CreateEntity("facing5", "cube", "rust");
	Transform* facing5T = facing5->GetComponent<Transform>();
	facing5T->SetLocalPosition(-8, 0, -1);
	Renderable* facing5R = facing5->GetComponent<Renderable>();
	RigidBody* facing5Rb = facing5->AddComponent<RigidBody>(facing5T, &(facing5R->BoundingBox()));
	facing5T->SetScale(0.4, 0.4, 0.4);

	Entity* facing5C = CreateEntity("facing5C", "cube", "rust");
	Transform* facing5CT = facing5C->GetComponent<Transform>();
	Renderable* facing5CR = facing5C->GetComponent<Renderable>();
	RigidBody* facing5CRb = facing5C->AddComponent<RigidBody>(facing5CT, &(facing5CR->BoundingBox()));
	facing5CT->SetParent(facing5T);
	facing5CT->SetScale(0.8, 0.8, 0.8);
	facing5CT->SetLocalPosition(0, 0, 0.6);

	//Target to be swarmed at
	Entity* target = CreateEntity("target", "cube", "water");
	Transform* targetT = target->GetComponent<Transform>();
	targetT->SetLocalPosition(-5, 0, 0);
	Renderable* targetR = target->GetComponent<Renderable>();
	RigidBody* targetRb = target->AddComponent <RigidBody>(targetT, &(targetR->BoundingBox()));
	//

	//light
	Entity* lights = CreateEntity("light");
	Lighting* l = lights->AddComponent<Lighting>(XMFLOAT4(-5, 0, 0, 0), XMFLOAT4(0.7f, 0, 0, 1), LightType::PointLight, 1, 8);
	Entity* lights2 = CreateEntity("light");
	Lighting* l2 = lights2->AddComponent<Lighting>(XMFLOAT4(0, 0, -10, 0), XMFLOAT4(0.7f, 0, 0, 1), LightType::PointLight, 1, 8);
	//cam
	cam.getViewMatrix();
	cam.setProjectionMatrix(800.0f / 600.0f);

	
}

void Scene::Tick(float deltaTime, float totalTime)
{
	cam.update(deltaTime);
	//auto* cam = GetEntityByName("facing");

	auto* tT = GetEntityByName("target")->GetComponent<Transform>();
	auto* tTR = GetEntityByName("target")->GetComponent<RigidBody>();

	static int moveDir = 1;

	if (tT->GetWorldPosition()->y < -6)
	{
		moveDir = 1;
	}

	else if (tT->GetWorldPosition()->y > 6)
	{
		moveDir = -1;
	}

	tT->SetWorldPosition(tT->GetWorldPosition()->x + 0.5f*deltaTime, tT->GetWorldPosition()->y + 2.0f*deltaTime * moveDir, tT->GetWorldPosition()->z + 2.0f*deltaTime * moveDir);


	auto* t1 = GetEntityByName("facing1")->GetComponent<Transform>();
	auto* tR1 = GetEntityByName("facing1")->GetComponent<RigidBody>();
	tR1->ProjectileSwarmingAt("target", 3.0f, 500, 1000, 20);
	tR1->Update(deltaTime);

	auto* t2 = GetEntityByName("facing2")->GetComponent<Transform>();
	auto* tR2 = GetEntityByName("facing2")->GetComponent<RigidBody>();
	tR2->ProjectileSwarmingAt("target", 3.0f, 500, 1000, 20);
	tR2->Update(deltaTime);

	auto* t3 = GetEntityByName("facing3")->GetComponent<Transform>();
	auto* tR3 = GetEntityByName("facing3")->GetComponent<RigidBody>();
	tR3->ProjectileSwarmingAt("target", 3.0f, 500, 1000, 20);
	tR3->Update(deltaTime);

	auto* t4 = GetEntityByName("facing4")->GetComponent<Transform>();
	auto* tR4 = GetEntityByName("facing4")->GetComponent<RigidBody>();
	tR4->ProjectileSwarmingAt("target", 3.0f, 500, 1000, 20);
	tR4->Update(deltaTime);

	auto* t5 = GetEntityByName("facing5")->GetComponent<Transform>();
	auto* tR5 = GetEntityByName("facing5")->GetComponent<RigidBody>();
	tR5->ProjectileSwarmingAt("target", 3.0f, 500, 1000, 20);
	tR5->Update(deltaTime);

}

void Scene::Exit()
{
	for (Entity* e : entities) {
		delete e;
	}
	entities.clear();


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
	m_EntityInfo e;
	e.name = name;
	e.pointer = ent;
	entity.push_back(e);
	AddEntity(ent);
	return ent;
}

Entity * Scene::CreateEntity(string name, string mesh, string material)
{
	Entity* ent = new Entity(name, GetMaterial(material).pointer, GetMesh(mesh).pointer);
	ent->SetMaterial(material);
	ent->SetMesh(mesh);
	m_EntityInfo e;
	e.name = name;
	e.mesh = mesh;
	e.material = material;
	e.pointer = ent;
	entity.push_back(e);
	AddEntity(ent);
	return ent;
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

void Scene::Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer)
{
	writer.StartObject();
	writer.String("name");
	writer.String(name.c_str());
	writer.String("skybox");
	writer.StartObject();
	writer.String("material");
	writer.String(skyboxMaterial.c_str());
	writer.String("mesh");
	writer.String(skyboxMesh.c_str());
	writer.EndObject();

	//mesh
	writer.String("meshes");
	writer.StartArray();
	//for
	for (auto i : meshes) {
		writer.StartObject();

		writer.String("name");
		writer.String(i.name.c_str());
		
		writer.String("file");
		
		writer.String(string(i.file.begin(),i.file.end()).c_str());

		writer.EndObject();
	}
	//end for
	writer.EndArray();

	//shader
	writer.String("shaders");
	writer.StartArray();
	//for
	for (auto i : shaders) {
		writer.StartObject();
		writer.String("name");
		writer.String(i.name.c_str());
		writer.String("file");	
		writer.String(string(i.file.begin(), i.file.end()).c_str());
		writer.EndObject();
	}	
	//end for
	writer.EndArray();

	//texture
	writer.String("textures");
	writer.StartArray();
	//for
	for (auto i : textures) {
		writer.StartObject();
		writer.String("type");
		writer.String(i.type.c_str());
		writer.String("name");
		writer.String(i.name.c_str());
		writer.String("file");
		writer.String(string(i.file.begin(), i.file.end()).c_str());
		writer.EndObject();
	}
	//end for
	writer.EndArray();



	//material
	writer.String("materials");
	writer.StartArray();
	//for
	for (auto i : materials) {
		writer.StartObject();
		writer.String("name");
		writer.String(i.name.c_str());
		writer.String("shader");
		writer.String(i.shader.c_str());
		writer.String("texture");
		writer.String(i.texture.c_str());
		writer.EndObject();
	}
	//end for
	writer.EndArray();


	//entity/transform
	writer.String("entities");
	writer.StartArray();
	componentManager->root->StartSerialize(writer);
	writer.EndArray();
	writer.EndObject();
}

void Scene::Build(rapidjson::Document &d)
{
	this->name = d["name"].GetString();	

	//cam
	cam.getViewMatrix();
	cam.setProjectionMatrix(800.0f / 600.0f);
	//mesh
	Value& b_meshes = d["meshes"];
	for (size_t i = 0; i < b_meshes.Size(); i++) {
		auto& b_mesh = b_meshes[i].GetObject();
		Wstr ws(b_mesh["file"].GetString());
		AddMesh(b_mesh["name"].GetString(),ws());
	}

	//shader
	Value& b_shaders = d["shaders"];
	for (size_t i = 0; i < b_shaders.Size(); i++) {
		auto& b_shader = b_shaders[i].GetObject();
		AddShader(b_shader["name"].GetString(), Wstr(b_shader["file"].GetString())());
	}
	//tex
	Value& b_textures = d["textures"];
	for (size_t i = 0; i < b_textures.Size(); i++) {
		auto& b_texture = b_textures[i].GetObject();
		AddTexture(b_texture["type"].GetString(), b_texture["name"].GetString(), Wstr(b_texture["file"].GetString())());
	}
	//mat
	Value& b_materials = d["materials"];
	for (size_t i = 0; i < b_materials.Size(); i++) {
		auto& b_material = b_materials[i].GetObject();
		AddMaterial(b_material["name"].GetString(), b_material["shader"].GetString(), b_material["texture"].GetString());
	}
	//skybox
	auto& b_skybox = d["skybox"];
	skybox = sceneGraph.CreateRenderable();
	skybox->SetMaterial(GetMaterial(b_skybox["material"].GetString()).pointer);
	skybox->SetMesh(GetMesh(b_skybox["mesh"].GetString()).pointer);
	skyboxMaterial = b_skybox["material"].GetString();
	skyboxMesh = b_skybox["mesh"].GetString();
	sceneGraph.SetSkyBox(skybox);
	//ent
	Value& b_entities = d["entities"];
	for (size_t i = 0; i < b_entities.Size(); i++) {
		auto& b_entity = b_entities[i].GetObject();
		Entity* e = CreateEntity(b_entity["name"].GetString());
		auto& b_components = b_entity["components"];
		for (size_t i = 0; i < b_components.Size(); i++) {
			auto& b_component = b_components[i].GetObject();
			string componentName = b_component["name"].GetString();
			if (componentName == "Transform") {
				Transform* t = e->GetComponent<Transform>();
				string parent = b_component["parent"].GetString();
				if (parent == "root") {
					t->SetParent(t);
				}
				else {
					t->SetParent(GetEntityByName(parent)->GetComponent<Transform>());
				}
				t->Load(b_component);
			}
			else if (componentName == "Renderable") {
				Renderable* r = e->AddComponent<Renderable>(GetMesh(b_entity["mesh"].GetString()).pointer,GetMaterial(b_entity["material"].GetString()).pointer);
				e->SetMaterial(b_entity["material"].GetString());
				e->SetMesh(b_entity["mesh"].GetString());
				r->Load(b_component);
			}else if(componentName == "Lighting"){
				XMFLOAT4 position(b_component["PositionX"].GetFloat(), b_component["PositionY"].GetFloat(), b_component["PositionZ"].GetFloat(), b_component["PositionW"].GetFloat());
				XMFLOAT4 color(b_component["ColorX"].GetFloat(), b_component["ColorY"].GetFloat(), b_component["ColorZ"].GetFloat(), b_component["ColorW"].GetFloat());
				Lighting* l = e->AddComponent<Lighting>(position,color,b_component["lightType"].GetInt(), b_component["enabled"].GetInt(),b_component["specularAmount"].GetInt());
			}
		}
	}
}




Mesh * Scene::AddMesh(string meshName, const wchar_t * file)
{
	RenderingSystem& renderer = *RenderingSystem::instance();
	Mesh* mesh = renderer.CreateMesh(file);
	m_MeshInfo m;
	m.name = meshName;
	m.file = file;
	m.pointer = mesh;
	meshes.push_back(m);
	return mesh;
}

m_MeshInfo Scene::GetMesh(string name)
{
	for (auto i : meshes) {
		if (i.name == name) {
			return i;
		}
	}
	return m_MeshInfo();
}

Material * Scene::AddMaterial(string name, string shader, string texture)
{
	RenderingSystem& renderer = *RenderingSystem::instance();
	Material* material = renderer.CreateMaterial(GetShader(shader).pointer);
	m_TextureInfo m2 = GetTexture(texture);
	material->SetTexture(GetTexture(texture).type, GetTexture(texture).pointer);
	m_MaterialInfo m;
	m.name = name;
	m.shader = shader;
	m.texture = texture;
	m.pointer = material;
	materials.push_back(m);
	return material;
}

m_MaterialInfo Scene::GetMaterial(string name)
{
	for (auto i : materials) {
		if (i.name == name) {
			return i;
		}
	}
	return m_MaterialInfo();
}

Shader * Scene::AddShader(string name, const wchar_t * file)
{
	RenderingSystem& renderer = *RenderingSystem::instance();
	Shader* shader = renderer.CreateShader(file);
	m_ShaderInfo s;
	s.name = name;
	s.file = file;
	s.pointer = shader;
	shaders.push_back(s);
	return shader;
}

m_ShaderInfo  Scene::GetShader(string name)
{
	for (auto i : shaders) {
		if (i.name == name) {
			return i;
		}
	}
	return m_ShaderInfo();
}

Texture * Scene::AddTexture(string type, string name, const wchar_t * file)
{
	RenderingSystem& renderer = *RenderingSystem::instance();
	Texture* texture = renderer.CreateTexture(file);
	m_TextureInfo t;
	t.name = name;
	t.file = file;
	t.type = type;
	t.pointer = texture;
	textures.push_back(t);
	return texture;
}

m_TextureInfo  Scene::GetTexture(string name)
{
	for (auto i : textures) {
		if (i.name == name) {
			return i;
		}
	}
	return m_TextureInfo();
}

