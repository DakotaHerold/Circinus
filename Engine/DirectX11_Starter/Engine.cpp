#include "Engine.h"

#include "NativeWindow.h"
#include "RenderingSystem.h"
#include "TransformSystem.h"
#include "Scene.h"
#include "Entity.h"
#include "SceneManager.h"

#ifdef HAS_EDITOR
#include "Editor.h"
#endif

using std::vector;

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

	// FIXME: get world entities 
	//entities = new vector<Entity *>();

	//Entity *entity = new Entity();
	//entities->push_back(entity);
	
	renderingSystem = new RenderingSystem();
	transformSystem = new TransformSystem();
	//physicsSystem = new PhysicsSystem();

	if (!renderingSystem->Init(nativeWindow))
		return false;

	inputManager = &InputManager::instance();
	inputManager->SetWindowHandle(nativeWindow->GetWindowHandle());

#ifdef HAS_EDITOR
	Editor* editor = new Editor();
	editor->Init(); // singleton initilized, it's ok to leave the pointer
#endif

	// TODO !
	// hardcoded debug code, 
	// scene loading should be in scripts or config file
	// and should be done by SceneManager
	currentScene = SceneManager::CreateNewScene("scene");
	currentScene->Enter();
	SceneManager::SaveScene(currentScene);
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

#ifdef HAS_GUI
		(GUI::instance().Update(nativeWindow->GetWindowWidth(), nativeWindow->GetWindowHeight(), &running));
#endif

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

		// TODO
		//Transform System
		transformSystem->update(deltaTime, currentScene->componentManager->GetAllComponents<Transform>());

		// scripting system
		// TODO

		
#ifdef HAS_EDITOR
		Editor::instance()->Update(deltaTime, totalTime);
#else
		// TODO !
		// hardcoded debug code, 
		// this should be done by SceneManager
		currentScene->Tick(deltaTime, totalTime);
#endif

		// particle system update
		renderingSystem->Update(deltaTime, totalTime);

		// rendering System
#ifdef HAS_EDITOR
		renderingSystem->DrawScene(Editor::instance()->GetEditorCamera(), currentScene->GetSceneGraph());
#else
		renderingSystem->DrawScene(currentScene->GetCamera(), currentScene->GetSceneGraph());
#endif

		// TODO
		//physicsSystem->update(deltaTime, currentScene->componentManager->rigidBodyComponents);
	}

	// GUI Cleanup
#ifdef HAS_GUI
	GUI::instance().End();
#endif

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

#ifdef HAS_EDITOR
	Editor::instance()->CleanUp();
#endif

	delete renderingSystem;
	delete transformSystem;
	//delete physicsSystem;
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
