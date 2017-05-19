#pragma once

#include "Config.h"


class NativeWindow;
class RenderingSystem;
class InputManager;
class Scene;
class TransformSystem;
class PhysicsSystem;
class Entity;
class SceneManager;

#include <vector>
#include <string>

class Engine
{
private:
	static Engine* _instance;
public:
	inline static Engine* instance() { return _instance; }
	inline Scene * GetCurScene() { return currentScene; }

public:
	Engine()
		:
		running(false),
		nativeWindow(nullptr),
		renderingSystem(nullptr),
		perfCounterSeconds(0.0),
		currentTime(0),
		previousTime(0),
		startTime(0),
		deltaTime(0.0f),
		totalTime(0.0f)
	{}

	Engine(const Engine&) = delete;
	Engine(Engine&&) = delete;
	Engine& operator = (const Engine&) = delete;
	Engine& operator = (Engine&&) = delete;

	bool Init();

	int Run();

	void LoadScene(std::string name);

	void SaveScene();

	void InitScene();

private:

	void Quit(int exitCode);

	void CleanUp();
		
private:
	bool				running;

	NativeWindow*		nativeWindow;
	RenderingSystem*	renderingSystem;
	PhysicsSystem*		physicsSystem;
	TransformSystem*	transformSystem;
	InputManager*		inputManager;

	Scene*				currentScene;

	std::vector<Entity *>*	entities;

private:
	void		InitTimer();
	void		UpdateTimer();

private:
	double		perfCounterSeconds;
	long long	currentTime;
	long long	previousTime;
	long long	startTime;

	float		deltaTime;
	float		totalTime;

};