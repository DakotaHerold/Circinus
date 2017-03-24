#include "InputManager.h"

InputManager::InputManager()
	: gamePad(new GamePad), tracker(new GamePad::ButtonStateTracker)
{
	// Initial check to see if controller is plugged in.
	auto state = gamePad->GetState(0); 
	(state.IsConnected()) ? gamePadEnabled = true : gamePadEnabled = false; 
	
	fireLaser = false;
	fireMissile = false;
}


void InputManager::UpdateInput(float deltaTime)
{
	// Check if quit key was pressed
	if (GetAsyncKeyState(VK_ESCAPE))
	{ 
		quit = true; 
	}

	// Get current state of gamepad  ---------------------------------------------

	// Note: GetState has different states for deadzones: DEAD_ZONE_NONE, DEAD_ZONE_INDEPENDENT_AXES, DEAD_ZONE_CIRCULAR. Independent is best for fine grain control such as steering. 
	// GetState Initializes with Independent
	auto state = gamePad->GetState(0);
	// Check if controller is still connected
	(state.IsConnected()) ? gamePadEnabled = true : gamePadEnabled = false;

	// If Gamepad is connected update for gamepad. Otherwise update for keyboard
	if (gamePadEnabled)
	{
		// Check if quit key was pressed on controller 
		if(state.IsViewPressed())
		{
			quit = true;
		}

		// Update Tracker
		tracker->Update(state);

		// Update Right thumbstick input
		controllerLookMoveX = state.thumbSticks.rightX;
		controllerLookMoveY = -state.thumbSticks.rightY;
		// Update Left Thumbstick / DPAD Input
		movingForward = state.IsLeftThumbStickUp() || state.IsDPadUpPressed();
		movingLeft = state.IsLeftThumbStickLeft() || state.IsDPadLeftPressed();
		movingBackward = state.IsLeftThumbStickDown() || state.IsDPadDownPressed();
		movingRight = state.IsLeftThumbStickRight() || state.IsDPadRightPressed();
		// Triggers
		ascending = state.IsRightTriggerPressed();
		descending = state.IsLeftTriggerPressed();

		// A and B. Uses Button State tracker to only set for single press
		fireLaser = (tracker->a == GamePad::ButtonStateTracker::PRESSED) ? true : false;
		fireMissile = (tracker->b == GamePad::ButtonStateTracker::PRESSED) ? true : false;



	}
	// Keyboard Input. Mouse is updated by event handler in Main.cpp
	else
	{
		movingForward = (GetAsyncKeyState('W') & 0x8000) ? true : false;
		movingLeft = (GetAsyncKeyState('A') & 0x8000) ? true : false;
		movingBackward = (GetAsyncKeyState('S') & 0x8000) ? true : false;
		movingRight = (GetAsyncKeyState('D') & 0x8000) ? true : false;
		ascending = (GetAsyncKeyState(VK_SPACE) & 0x8000) ? true : false;
		descending = (GetAsyncKeyState('X') & 0x8000) ? true : false;
		
		fireLaser = (GetKeyState(VK_CONTROL) & 0x8000) ? true : false;
		fireMissile = (GetKeyState(VK_LSHIFT) & 0x8000) ? true : false;
		
	}

	// Testing 
	/*if (fireLaser)
	{
		cout << "Fired mah lazer!" << endl;
	}
	if (fireMissile)
	{
		cout << "Fired missile!" << endl;
	}*/
}

//GamePad::ButtonStateTracker::ButtonState InputManager::GetA()
//{
//	auto state = gamePad->GetState(0);
//	if (state.IsConnected())
//	{
//		tracker->Update(state); 
//	}
//	return tracker->a; 
//}
