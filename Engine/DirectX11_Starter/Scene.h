#pragma once
#include <rapidjson\prettywriter.h>
#include <rapidjson\document.h>
#include <vector>
#include "SceneGraph.h"
#include "ComponentManager.h"
#include "Camera.h"


class Entity;
class Material;

typedef vector<Entity*> EntityVector;
//#define GetObject GetObject();
#undef GetObject;
struct m_MaterialInfo
{
	string name;
	string shader;
	string texture;
	Material* pointer;
};

struct m_MeshInfo {
	string name;
	const char * file;
	Mesh* pointer;
};

struct m_ShaderInfo {
	string name;
	const wchar_t * file;
	Shader* pointer;
};

struct m_TextureInfo {
	string name;
	string type;
	const wchar_t * file;
	Texture* pointer;

};

struct m_EntityInfo {
	string name;
	bool isRenderable;
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
	SceneGraph* GetSceneGraph() { return &sceneGraph; }
	ComponentManager* componentManager;


	EntityVector GetAllEntities();

	void AddEntity(Entity* entity);

	Entity* CreateEntity(string name);

	Entity* CreateEntity(string name, string mesh, string material);

	bool DeleteEntityByName(string name);

	Entity* GetEntityByName(string name);

	Entity* GetEntityByID(int id);

	string GetName() { return name; }

	void Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer);
		
	void Build(rapidjson::Document &d);



private:
	Camera			cam;

	SceneGraph		sceneGraph;

	string			name="Scene1";


	char* WcharToChar(const wchar_t* wp);

	wchar_t* CharToWchar(const char* c);

	vector<m_MeshInfo> meshes;

	Mesh* AddMesh(string meshName, const char * file);

	m_MeshInfo GetMesh(string name);

	vector<m_MaterialInfo> materials;

	Material* AddMaterial(string name,string shader,string texture);

	m_MaterialInfo GetMaterial(string name);

	vector<m_ShaderInfo> shaders;

	Shader* AddShader(string name,const wchar_t * file);

	m_ShaderInfo GetShader(string name);

	vector<m_TextureInfo> textures;

	Texture* AddTexture(string type, string name, const wchar_t * file);

	m_TextureInfo GetTexture(string name);

	vector<Entity*> entities;

	vector<m_EntityInfo> entity;

	Entity*			lights;

	Renderable* skybox;

	string skyboxMesh;

	string skyboxMaterial;

};
