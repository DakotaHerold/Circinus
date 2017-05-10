#pragma once

extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}


#include "Component.h"
#include "RigidBody.h"
#include <LuaBridge.h>
#include <iostream>

class ScriptComponent :
	public Component
{
public:
	void Update() override; 
	//void Release() override; 

	ScriptComponent(std::string scriptFile, RigidBody* body);
	~ScriptComponent();

private: 
	// LuaBridge Attributes
	lua_State* L; 
	//LuaRef initFunc;// , updateFunc, releaseFunc;

	RigidBody* rigidbody; 
	// transform variables for lua
	float x, y, z; 

	// LUA TESTING VARIABLES/FUNCTIONS
	int globalVar;
	static float staticVar;

	std::string stringProperty;
	std::string getString() { return stringProperty; }
	void setString(std::string s) { stringProperty = s; }

	int foo() { return 42; }
	void bar(char const*) { }
	int cFunc(lua_State* L) { return 0; }
	//std::string luaString; 
	//int answer;
};

