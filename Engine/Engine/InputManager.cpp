#include "InputManager.h"

InputManager::InputManager()
	: gamePad(new GamePad), tracker(new GamePad::ButtonStateTracker)
{
	auto state = gamePad->GetState(0); 
	(state.IsConnected()) ? gamePadEnabled = true : gamePadEnabled = false; 
	
}


//InputManager::~InputManager()
//{
//}

GamePad::ButtonStateTracker::ButtonState InputManager::GetA()
{
	auto state = gamePad->GetState(0);
	if (state.IsConnected())
	{
		tracker->Update(state); 
	}
	return tracker->a; 
}
