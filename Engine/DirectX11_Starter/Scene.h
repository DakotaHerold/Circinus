#pragma once
#include <rapidjson\prettywriter.h>
#include <rapidjson\document.h>
#include <vector>
#include "ComponentManager.h"
#include "Camera.h"
#include "Renderable.h"

class Entity;
class Material;

typedef vector<Entity*> EntityVector;
//#define GetObject GetObject();
#undef GetObject
struct m_MaterialInfo
{
	string name;
	string shader;
	string texture;
	Material* pointer;
};

struct m_MeshInfo {
	string name;
	wstring file;
	Mesh* pointer;
};

struct m_ShaderInfo {
	string name;
	wstring file;
	Shader* pointer;
};

struct m_TextureInfo {
	string name;
	string type;
	wstring file;
	Texture* pointer;

};

struct m_EntityInfo {
	string name;
	string mesh;
	string material;
	Entity* pointer;
};

// 
//		NOTE:
//		This is just a hardcoded stub class, for debug
//		Replace this if it's your working area!
//

using namespace rapidjson;

class Scene
{
public:
	Scene();

	void Enter();

	void Tick(float deltaTime, float totalTime);

	void Exit();

	Camera* GetCamera() { return &cam; }

	ComponentManager* componentManager;


	EntityVector GetAllEntities();

	void AddEntity(Entity* entity);

	Entity* CreateEntity(string name);

	Entity* CreateEntity(string name, string mesh, string material);

	bool DeleteEntityByName(string name);

	Entity* GetEntityByName(string name);

	Entity* GetEntityByID(int id);

	string GetName() { return name; }

	Renderable* GetSkyBox() { return skybox; }

	void Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer);
		
	void Build(rapidjson::Document &d);

	Transform* root;

private:
	Camera			cam;

	string			name;

	vector<m_MeshInfo> meshes;

	vector<m_MaterialInfo> materials;

	vector<m_ShaderInfo> shaders;

	vector<m_TextureInfo> textures;

	vector<Entity*> entities;

	vector<m_EntityInfo> entity;

	Renderable* skybox;

	string skyboxMesh;

	string skyboxMaterial;


private:


	Mesh* AddMesh(string meshName, const wchar_t * file);

	m_MeshInfo GetMesh(string name);

	Material* AddMaterial(string name, string shader, string texture);

	m_MaterialInfo GetMaterial(string name);

	Shader* AddShader(string name, const wchar_t * file);

	m_ShaderInfo GetShader(string name);

	Texture* AddTexture(string type, string name, const wchar_t * file);

	m_TextureInfo GetTexture(string name);
};
