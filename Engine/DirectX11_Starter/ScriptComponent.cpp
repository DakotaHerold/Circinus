#include "ScriptComponent.h"




ScriptComponent::ScriptComponent()
{
	//type = ComponentType::ScriptComp;
	//L = luaL_newstate();
	//luaL_dofile(L, "script.lua");
	//luaL_openlibs(L);
	//LuaRef s = getGlobal(L, "testString");
	//LuaRef n = getGlobal(L, "number");
	//luaString = s.cast<std::string>();
	//answer = n.cast<int>();
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
