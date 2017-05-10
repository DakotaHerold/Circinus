#include "InputManager.h"

InputManager::InputManager()
	:
	gamePad(new GamePad),
	tracker(new GamePad::ButtonStateTracker),
	mouse(new Mouse),
	keyboard(new Keyboard),
	mouseMoveX(0.0f),
	mouseMoveY(0.0f)

{
	// Initial check to see if controller is plugged in.
	auto state = gamePad->GetState(0); 
	(state.IsConnected()) ? gamePadEnabled = true : gamePadEnabled = false; 

	// Initialize Keyboard keys 
	laserKey.vKey = 0x11; 
	laserKey.keyState = 0; 

	missileKey.vKey = 0xA0; 
	missileKey.keyState = 0; 
}


void InputManager::SetWindowHandle(void * handle)
{
	mouse->SetWindow((HWND)handle);
	mouse->SetMode(Mouse::MODE_ABSOLUTE);
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
		if (state.IsViewPressed())
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

		Mouse::State m = mouse->GetState();
		leftMouseHeld = m.leftButton;
		rightMouseHeld = m.rightButton;
		middleMouseHeld = m.middleButton;

		prevMouseWheelValue = mouseWheelValue;
		mouseWheelValue = static_cast<float>(m.scrollWheelValue);

		if (m.positionMode == Mouse::MODE_RELATIVE)
		{
			mouseMoveX = static_cast<float>(m.x);
			mouseMoveY = static_cast<float>(m.y);
		}
		else
		{
			mousePosX = static_cast<float>(m.x);
			mousePosY = static_cast<float>(m.y);
		}

		if (leftMouseHeld || rightMouseHeld || middleMouseHeld)
		{
			mouse->SetMode(Mouse::MODE_RELATIVE);
		}
		else
		{
			mouse->SetMode(Mouse::MODE_ABSOLUTE);
		}
	}

	// Testing 
	//if (fireLaser)
	//{
	//	cout << "Fired laser!" << endl;
	//}
	//if (fireMissile)
	//{
	//	cout << "Fired missile!" << endl;
	//}
}

bool InputManager::GetKeyDown(KeyInfo& key)
{
	if ((GetAsyncKeyState(key.vKey) & 0x8000) && (key.keyState != 1))
	{
		key.keyState = KeyStates::PRESSED;
		return true;
	}

	if (!(GetAsyncKeyState(key.vKey) & 0x8000))
	{
		key.keyState = KeyStates::RELEASED;
	}

	return false;
}

bool InputManager::GetKeyHolding(KeyInfo& key)
{
	if ((GetAsyncKeyState(key.vKey) & 0x8000))
	{
		key.keyState = KeyStates::HELD;
		return true;
	}

	key.keyState = KeyStates::RELEASED;
	return false;
}
