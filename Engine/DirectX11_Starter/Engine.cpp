#include "Engine.h"

#include "NativeWindow.h"
#include "RenderingSystem.h"
#include "Scene.h"

Engine* Engine::_instance = nullptr;

bool Engine::Init()
{
	if (nullptr != _instance)
		return false;

	// other module could use this;
	_instance = this;

	nativeWindow = new NativeWindow();

	if (!nativeWindow->Init())
		return false;

	renderingSystem = new RenderingSystem();

	if (!renderingSystem->Init(nativeWindow))
		return false;

	inputManager = &InputManager::instance();
	inputManager->SetWindowHandle(nativeWindow->GetWindowHandle());

	// TODO !
	// hardcoded debug code, 
	// scene loading should be in scripts or config file
	// and should be done by SceneManager
	currentScene = new Scene();
	currentScene->Enter();

	return true;
}

int Engine::Run()
{
	running = true;

	InitTimer();

	while (running)
	{
		// deltaTime and totalTime
		UpdateTimer();
		
		// native window event processiing
		nativeWindow->ProcessEvent();
		nativeWindow->CalculateFrameStats(totalTime);
		if (nativeWindow->WindowIsClosed())
		{
			running = false;
		}
		
		// process input events
		inputManager->UpdateInput(deltaTime);
		if (inputManager->GetQuit())
		{
			Quit(0);
		}

		// physics system
		// TODO

		// scripting system
		// TODO


		// TODO !
		// hardcoded debug code, 
		// this should be done by SceneManager
		currentScene->Tick(deltaTime, totalTime);

		// rendering System
		// TODO
		renderingSystem->DrawScene(currentScene->GetCamera(), currentScene->GetSceneGraph());
	}

	CleanUp();

	return 0;
}

void Engine::Quit(int exitCode)
{
	PostQuitMessage(exitCode);
}

void Engine::CleanUp()
{
	// TODO !
	// hardcoded debug code, 
	// this should be done by SceneManager
	currentScene->Exit();
	delete currentScene;



	delete renderingSystem;
	delete nativeWindow;
}

void Engine::InitTimer()
{
	__int64 perfFreq;
	QueryPerformanceFrequency((LARGE_INTEGER*)&perfFreq);
	perfCounterSeconds = 1.0 / (double)perfFreq;

	__int64 now;
	QueryPerformanceCounter((LARGE_INTEGER*)&now);
	startTime = now;
	currentTime = now;
	previousTime = now;
}

void Engine::UpdateTimer()
{
	__int64 now;
	QueryPerformanceCounter((LARGE_INTEGER*)&now);
	currentTime = now;

	deltaTime = max((float)((currentTime - previousTime) * perfCounterSeconds), 0.0f);

	// Calculate the total time from start to now
	totalTime = (float)((currentTime - startTime) * perfCounterSeconds);

	// Save current time for next frame
	previousTime = currentTime;
}
