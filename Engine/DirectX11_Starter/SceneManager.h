#pragma once
#include "Scene.h"
#include <rapidjson\prettywriter.h>
#include <rapidjson\document.h>
#include <rapidjson\filereadstream.h>
#include <rapidjson\filewritestream.h>
using namespace rapidjson;
class SceneManager
{
public:
	SceneManager();
	~SceneManager();
	static Scene* CreateNewScene(string name);
	static Scene* LoadScene(string path);
	static Scene* SaveScene(Scene* scene);
	
};

