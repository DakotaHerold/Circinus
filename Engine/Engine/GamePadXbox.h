#pragma once
// Credit to David Amador for the controller input 
// http://www.david-amador.com/2012/04/xbox-360-controller-input-in-c-via-xinput/
#define WIN32_LEAN_AND_MEAN // We don't want the extra stuff like MFC and such
#include <Windows.h>
#include <Xinput.h> // XInput API
#include <DirectXMath.h>
#include "DirectXGameCore.h"
#pragma comment(lib, "XInput.lib")   // Library. If your compiler doesn't support this type of lib include change to the corresponding one
using namespace DirectX;


// Button definitions 
typedef enum
{
	GamePad_Button_DPAD_UP = 0,
	GamePad_Button_DPAD_DOWN = 1,
	GamePad_Button_DPAD_LEFT = 2,
	GamePad_Button_DPAD_RIGHT = 3,
	GamePad_Button_START = 4,
	GamePad_Button_BACK = 5,
	GamePad_Button_LEFT_THUMB = 6,
	GamePad_Button_RIGHT_THUMB = 7,
	GamePad_Button_LEFT_SHOULDER = 8,
	GamePad_Button_RIGHT_SHOULDER = 9,
	GamePad_Button_A = 10,
	GamePad_Button_B = 11,
	GamePad_Button_X = 12,
	GamePad_Button_Y = 13,
	GamePadButton_Max = 14
}GamePadButton;

// GamePad Indexes
typedef enum
{
	GamePadIndex_One = 0,
	GamePadIndex_Two = 1,
	GamePadIndex_Three = 2,
	GamePadIndex_Four = 3,
}GamePadIndex;

// The GamePad State Stuct, were we store the buttons positions
struct GamePadState
{
	bool		_buttons[GamePadButton_Max];
	XMFLOAT2 	_left_thumbstick;               // <= I'm using a Vector2 (floats) class but you can replaced it with a float X and Y or whatever your Vector2 class is
	XMFLOAT2	        _right_thumbstick;
	float		_left_trigger;
	float		_right_trigger;
	// Just to clear all values to default
	void reset()
	{
		for (int i = 0; i<(int)GamePadButton_Max; ++i) _buttons[i] = false;
		_left_thumbstick = XMFLOAT2(0.0f, 0.0f);
		_right_thumbstick = XMFLOAT2(0.0f, 0.0f);
		_left_trigger = _right_trigger = 0.0f;
	}
};

class GamePadXbox
{
public:
	GamePadXbox(GamePadIndex player)
	{
		_playerIndex = player;
		State.reset();
	}

	virtual ~GamePadXbox(void)
	{
		// We don't want the controller to be vibrating accidentally when we exit the app
		if (is_connected()) vibrate(0.0f, 0.0f);
	}

	bool is_connected();
	void vibrate(float leftmotor = 0.0f, float rightmotor = 0.0f);
	void update();

public:
	GamePadState	State;
private:
	XINPUT_STATE _controllerState;
	GamePadIndex _playerIndex;

};

