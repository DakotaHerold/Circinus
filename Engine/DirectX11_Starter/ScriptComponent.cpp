#include "ScriptComponent.h"


ScriptComponent::ScriptComponent()
{
	//type = ComponentType::ScriptComp;
	lua_State* L = luaL_newstate();
	luaL_dofile(L, "Scripts/script.lua");
	//luaL_dostring(L, "testString = \"Works\" \nnumber = 42"); 
	luaL_openlibs(L);
	lua_pcall(L, 0, 0, 0);
	LuaRef s = getGlobal(L, "testString");
	LuaRef n = getGlobal(L, "number");
	std::string luaString = s.cast<std::string>();
	int answer = n.cast<int>();
	std::cout << luaString << std::endl;
	std::cout << "And here's our number:" << answer << std::endl;
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


ScriptComponent::~ScriptComponent()
{
}
