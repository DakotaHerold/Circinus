#include "InputManager.h"

InputManager::InputManager()
	: gamePad(new GamePad), tracker(new GamePad::ButtonStateTracker)
{
	// Initial check to see if controller is plugged in.
	auto state = gamePad->GetState(0); 
	(state.IsConnected()) ? gamePadEnabled = true : gamePadEnabled = false; 
	
	
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

		// Update Right thumbstick Input
		controllerLookMoveX = state.thumbSticks.rightX;
		controllerLookMoveX = state.thumbSticks.rightY;
		//cout << controllerLookMoveX << endl;
		cout << GetMouseMoveX() << endl; 
	}
	// Keyboard and Mouse --------------------------------------------------------
	else
	{

	}

	
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
