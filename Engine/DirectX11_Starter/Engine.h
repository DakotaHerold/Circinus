#pragma once

class NativeWindow;
class RenderingSystem;
class InputManager;
class Scene;

class Engine
{
private:
	static Engine* _instance;
public:
	inline static Engine* instance() { return _instance; }

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

private:

	void Quit(int exitCode);

	void CleanUp();
		
private:
	bool				running;

	NativeWindow*		nativeWindow;
	RenderingSystem*	renderingSystem;
	InputManager*		inputManager;

	Scene*				currentScene;

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