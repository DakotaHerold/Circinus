#pragma once

extern "C" {
# include "lua.h"
# include "lauxlib.h"
# include "lualib.h"
}

#include "Component.h"
#include <LuaBridge.h>
#include <iostream>

using namespace luabridge;
using namespace std;

class ScriptComponent :
	public Component
{
public:
	void Update() override; 
	void Release() override; 

	ScriptComponent();
	~ScriptComponent();

private: 
	lua_State* L; 
	std::string luaString; 
	int answer;
};

