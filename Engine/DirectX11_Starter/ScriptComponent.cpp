#include "ScriptComponent.h"
using namespace luabridge;

ScriptComponent::ScriptComponent(string scriptFile)
{
	testNumber = 3;

	scriptFile = "script2.lua";

	// Define a Lua State object
	L = luaL_newstate();

	// Get script name and convert to C string 
	string fileName = "Scripts/" + scriptFile;
	luaL_dofile(L, fileName.c_str());

	//luaL_dofile(L, "Scripts/script.lua");
	//luaL_dostring(L, "testString = \"Works\" \nnumber = 42"); 
	luaL_openlibs(L);
	lua_pcall(L, 0, 0, 0);
	//LuaRef s = getGlobal(L, "testString");
	//LuaRef n = getGlobal(L, "number");
	//std::string luaString = s.cast<std::string>();
	//int answer = n.cast<int>();
	//std::cout << luaString << std::endl;
	//std::cout << "And here's our number:" << answer << std::endl;

	// Calling lua function example 
	cout << endl; 
	//LuaRef  updateFunc = getGlobal(L, "Update");
	//try {
	//updateFunc();
	//}
	//catch (LuaException const& e) {
	//	std::cerr && e.what();
	//}


	LuaRef testPrint = getGlobal(L, "var1");

	//testPrint();


	cout << "Script Initialized" << endl; 
	
	
	//lua_close(L);
	//delete L; 
}

void ScriptComponent::Update()
{

	//std::cout << luaString << std::endl;
	//std::cout << "And here's our number:" << answer << std::endl;
}

void ScriptComponent::Release()
{

}

void ScriptComponent::TestLua()
{
	cout << "This was called from Lua" << endl; 
}


ScriptComponent::~ScriptComponent()
{
	
}
