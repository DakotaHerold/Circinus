#include "ScriptComponent.h"
using namespace luabridge;
using namespace std;

ScriptComponent::ScriptComponent(string scriptFile, RigidBody* body)
{
	rigidbody = body; 

	// Define a Lua State object
	L = luaL_newstate();
	x = rigidbody->GetTransform()->GetLocalPosition()->x;
	y = rigidbody->GetTransform()->GetLocalPosition()->y;
	z = rigidbody->GetTransform()->GetLocalPosition()->z;
	// Get script name and convert to C string 
	fileName = "Scripts/" + scriptFile+".lua";
	file = scriptFile;
	luaL_dofile(L, fileName.c_str());
	luaL_openlibs(L);
	lua_pcall(L, 0, 0, 0);

	// TO-DO
	// Setup Lua Scripting API to Rigidbody 



	getGlobalNamespace(L)
		.beginNamespace("test")
		.addVariable("x", &x)
		.addVariable("y", &y)
		.addVariable("z", &z)

		.endNamespace();

	cout << "Script Initialized" << endl; 
	
	 
	luabridge::LuaRef initFunc = luabridge::getGlobal(L, "Initialize");
	// Call script init
	try {
		initFunc();
	}
	catch (LuaException const& e) {
		std::cerr && e.what();
	}
}

ScriptComponent::ScriptComponent()
{
}

void ScriptComponent::Init(std::string scriptFile, RigidBody * body)
{
	rigidbody = body;

	// Define a Lua State object
	L = luaL_newstate();
	x = rigidbody->GetTransform()->GetLocalPosition()->x;
	y = rigidbody->GetTransform()->GetLocalPosition()->y;
	z = rigidbody->GetTransform()->GetLocalPosition()->z;
	// Get script name and convert to C string 
	fileName = "Scripts/" + scriptFile + ".lua";
	file = scriptFile;
	luaL_dofile(L, fileName.c_str());
	luaL_openlibs(L);
	lua_pcall(L, 0, 0, 0);

	// TO-DO
	// Setup Lua Scripting API to Rigidbody 



	getGlobalNamespace(L)
		.beginNamespace("test")
		.addVariable("x", &x)
		.addVariable("y", &y)
		.addVariable("z", &z)

		.endNamespace();

	cout << "Script Initialized" << endl;


	luabridge::LuaRef initFunc = luabridge::getGlobal(L, "Initialize");
	// Call script init
	try {
		initFunc();
	}
	catch (LuaException const& e) {
		std::cerr && e.what();
	}
}

void ScriptComponent::ResetScript(std::string scriptFile)
{
	Init(scriptFile, rigidbody);
}

void ScriptComponent::Update()
{
	// Call Lua Update if one exists 
	luabridge::LuaRef updateFunc = luabridge::getGlobal(L, "Update");
	try {
		updateFunc();
	}
	catch (LuaException const& e) {
		std::cerr && e.what();
	}

	rigidbody->GetTransform()->SetLocalPosition(x, y, z);
}

ScriptComponent::~ScriptComponent()
{
	luabridge::LuaRef  releaseFunc = luabridge::getGlobal(L, "Release");
	try {
		releaseFunc();
	}
	catch (LuaException const& e) {
		std::cerr && e.what();
	}
	releaseFunc = luabridge::Nil();
	lua_close(L);
}

void ScriptComponent::Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer)
{
	writer.StartObject();
	writer.String("name");
	writer.String("Script");
	writer.String("file");
	writer.String(file.c_str());
	writer.EndObject();
}

void ScriptComponent::Load(rapidjson::Value v)
{

}
