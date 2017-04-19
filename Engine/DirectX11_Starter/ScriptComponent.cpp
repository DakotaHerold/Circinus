#include "ScriptComponent.h"
using namespace luabridge;

ScriptComponent::ScriptComponent(string scriptFile)
{

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

	getGlobalNamespace(L)
		.beginNamespace("test")
		.addVariable("var1", &globalVar)
		//.addVariable("var2", &staticVar, false)     // read-only
		//.addProperty("prop1", getString, setString)
		//.addProperty("prop2", getString)            // read only
		//.addFunction("foo", foo)
		//.addFunction("bar", bar)
		//.addCFunction("cfunc", cFunc)
		.endNamespace();


	// Calling lua function example 
	cout << endl; 
	LuaRef  updateFunc = getGlobal(L, "Update");
	//try {
	updateFunc();
	//}
	//catch (LuaException const& e) {
	//	std::cerr && e.what();
	//}

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
