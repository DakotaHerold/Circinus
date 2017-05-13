#pragma once

#include <Windows.h>
#include "GamePad.h"
#include "Keyboard.h"
#include "Mouse.h"
#include <iostream>

using namespace DirectX; 
using namespace std;

#pragma region Key State Management 
enum KeyStates
{
	RELEASED = 0,
	PRESSED,
	HELD
};

struct KeyInfo
{
	int vKey;
	SHORT keyState;
};
#pragma endregion

// Class Definition 
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
	bool GetLeftMouseDown() { return leftMouseDown; }
	bool GetRightMouseDown() { return rightMouseDown; }
	bool GetMiddleMouseDown() { return middleMouseDown; }
	bool GetLeftMouseUp() { return leftMouseUp; }
	bool GetRightMouseUp() { return rightMouseUp; }
	bool GetMiddleMouseUp() { return middleMouseUp; }
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

	// works when cursor is shown
	float GetMousePositionX() { return mousePosX; }
	float GetMousePositionY() { return mousePosY; }
	// works when cursor is hidden
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
	bool leftMouseDown;
	bool middleMouseDown;
	bool rightMouseDown;
	bool leftMouseUp;
	bool middleMouseUp;
	bool rightMouseUp;
	float mousePosX;
	float mousePosY;
	float mouseMoveX; 
	float mouseMoveY;
	float mouseWheelValue;
	float prevMouseWheelValue;

	// Keyboard 
	KeyInfo laserKey;
	KeyInfo missileKey; 

	#pragma endregion

	#pragma region Helper Functions
	bool GetKeyDown(KeyInfo& key);
	bool GetKeyHolding(KeyInfo& key);
	#pragma endregion


};

