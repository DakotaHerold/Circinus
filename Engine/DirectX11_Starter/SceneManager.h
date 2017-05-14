#pragma once
#include "Scene.h"
#include <rapidjson\prettywriter.h>
#include <rapidjson\document.h>
#include <rapidjson\filereadstream.h>
#include <rapidjson\filewritestream.h>
#include <fstream>
#include <rapidjson\rapidjson.h>
using namespace rapidjson;
using namespace std;
class SceneManager
{
public:
	SceneManager();
	~SceneManager();
	static Scene* CreateNewScene(string name);
	static Scene* LoadScene(string name);
	static Scene* SaveScene(Scene* scene);
	
};

