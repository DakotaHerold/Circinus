#pragma once

#include <Windows.h>
#include "GamePad.h"
#include "Keyboard.h"
#include "Mouse.h"
#include <iostream>

using namespace DirectX; 
using namespace std;

class InputManager
{
public: 
	// Singleton reference
	static InputManager& instance()
	{
		static InputManager input_instance;
		return input_instance;
	}
public:
	#pragma region Public Attributes
	#pragma endregion

	// Singleton destructors
	InputManager(InputManager const&) = delete;              
	void operator=(InputManager const&) = delete; 

#pragma region Getters and Setters
	// Getters
	bool GetQuit() { return quit;  }

	// Keyboard / Mouse 
	bool GetLeftMouseHeld() { return leftMouseHeld; }
	bool GetRightMouseHeld() { return rightMouseHeld; }
	bool GetMiddleMouseHeld() { return middleMouseHeld; }
	bool GetMovingForward() { return movingForward; }
	bool GetMovingBackward() { return movingBackward; }
	bool GetMovingLeft() { return movingLeft; }
	bool GetMovingRight() { return movingRight; }
	bool GetAscending() { return ascending; }
	bool GetDescending() { return descending; }

	// Controller
	bool GetGamePadEnabled() { return gamePadEnabled; }
	float GetControllerMoveX() { return controllerLookMoveX; }
	float GetControllerMoveY() { return controllerLookMoveY; }


	float GetMouseMoveX() { return mouseMoveX; }
	float GetMouseMoveY() { return mouseMoveY; }
	float GetMouseWheelDelta() { return mouseWheelValue - prevMouseWheelValue; }

#pragma endregion

#pragma region Class functions
	void SetWindowHandle(void* handle);

	void UpdateInput(float deltaTime);
#pragma endregion

private:
	// Constructor 
	InputManager();

	#pragma region Private Attributes

	bool gamePadEnabled; 
	unique_ptr<GamePad> gamePad;
	unique_ptr<GamePad::ButtonStateTracker> tracker; 
	
	unique_ptr<Mouse>		mouse;
	unique_ptr<Keyboard>	keyboard;

	bool quit; 

	// Movement Booleans
	bool movingForward; 
	bool movingBackward; 
	bool movingLeft; 
	bool movingRight; 
	bool ascending; 
	bool descending; 
	// Gameplay Booleans
	bool fireLaser; 
	bool fireMissile;

	// Controller Input.
	float controllerLookMoveX;
	float controllerLookMoveY;

	// Mouse Input. Set from Main's event calls as they are tied to Windows OS. 
	bool leftMouseHeld;
	bool middleMouseHeld;
	bool rightMouseHeld;
	float mouseMoveX; 
	float mouseMoveY;
	float mouseWheelValue;
	float prevMouseWheelValue;
	#pragma endregion

	#pragma region Helper Functions
	//GamePad::ButtonStateTracker::ButtonState GetA();
	#pragma endregion


};

