#pragma once

extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}


#include "Component.h"
#include <LuaBridge.h>
#include <iostream>


using namespace std;

class ScriptComponent :
	public Component
{
public:
	void Update() override; 
	void Release() override; 

	void TestLua(); 

	ScriptComponent(string scriptFile);
	~ScriptComponent();

private: 
	int testNumber; 


	lua_State* L; 
	std::string luaString; 
	int answer;
};

