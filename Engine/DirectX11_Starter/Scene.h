#pragma once

#include <vector>
#include "SceneGraph.h"
#include "ComponentManager.h"
#include "Camera.h"
#include <rapidjson\prettywriter.h>

class Entity;
class Material;

typedef vector<Entity*> EntityVector;

typedef vector<std::pair<string, Material*>> MaterialVector;

typedef vector<std::pair<string, Mesh*>> MeshVector;
// 
//		NOTE:
//		This is just a hardcoded stub class, for debug
//		Replace this if it's your working area!
//
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

	Entity* CreateEntity(string name, Mesh* mesh, Material* material);

	bool DeleteEntityByName(string name);

	Entity* GetEntityByName(string name);

	Entity* GetEntityByID(int id);

	//string GetName();

	void Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) {
		writer.StartObject();
		writer.String("name");
		writer.String("Scene");


		//mesh
		writer.String("meshs");
		writer.StartArray();
		//for
		writer.StartObject();
		writer.String("name");
		writer.String("cube");
		writer.String("path");
		writer.String("Assets/Models/cube.fbx");
		writer.EndObject();
		//end for

		writer.EndArray();

		//shader
		writer.String("shader");
		writer.StartArray();
		//for
		writer.StartObject();
		writer.String("name");
		writer.String("Opaque");
		writer.String("path");
		writer.String("Assets/ShaderObjs/Opaque.cso");
		writer.EndObject();
		//end for
		writer.EndArray();

		//texture
		writer.String("texture");
		writer.StartArray();
		//for
		writer.StartObject();
		writer.String("type");
		writer.String("texDiffuse");
		writer.String("name");
		writer.String("rust");
		writer.EndObject();

		writer.StartObject();
		writer.String("type");
		writer.String("texDiffuse");
		writer.String("name");
		writer.String("water");
		writer.EndObject();
		//end for
		writer.EndArray();

		//material
		writer.String("material");
		writer.StartArray();
		//for
		writer.StartObject();
		writer.String("name");
		writer.String("mat1");
		writer.String("shader");
		writer.String("Opaque");
		writer.String("texture");
		writer.String("rust");
		writer.EndObject();

		writer.StartObject();
		writer.String("name");
		writer.String("mat2");
		writer.String("shader");
		writer.String("Opaque");
		writer.String("texture");
		writer.String("water");
		writer.EndObject();
		//end for
		writer.EndArray();


		//entity/transform
		componentManager->root->StartSerialize(writer);
		writer.EndObject();
	}



private:
	Camera			cam;

	SceneGraph		sceneGraph;

	string			name;

	MeshVector		meshes;

	Mesh* AddMesh(string meshName, string file);

	Mesh* GetMesh(string name);

	MaterialVector	materials;

	Material* AddMaterial(string materialName, const wchar_t * shaderFile, string textureName, const wchar_t * textureFile);

	Material* GetMaterial(string name);

	Material*		mat;

	vector<Entity*> entities;

	Entity*			lights;
};
