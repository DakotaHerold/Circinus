#pragma once
#include "GamePad.h"
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
	GamePad::ButtonStateTracker::ButtonState GetA(); 
	#pragma endregion

private:
	// Constructor 
	InputManager();

	#pragma region Private Attributes

	bool gamePadEnabled; 
	unique_ptr<GamePad> gamePad;
	unique_ptr<GamePad::ButtonStateTracker> tracker; 
	
	// Specific and may need to be abstracted 
	bool movingForward; 
	bool movingBackward; 
	bool movingLeft; 
	bool movingRight; 
	bool ascending; 
	bool descending; 
	#pragma endregion
};

