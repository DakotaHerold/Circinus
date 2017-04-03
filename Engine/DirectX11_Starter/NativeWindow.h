#pragma once

#include <Windows.h>
#include <string>
#include <functional>

class NativeWindow
{
public:
	NativeWindow();

	bool Init();

	int ProcessEvent();

	float GetTotalTime() const { return totalTime; }
	float GetDeltaTime() const { return deltaTime; }

	void* GetWindowHandle() const { return (void*)hMainWnd; }

	bool WindowIsClosed() const { return quit; }

	void SetResizeCallback(const std::function<void(int, int)>& func) { callbackOnResize = func; }

private:
	// Handles window initialization
	bool InitMainWindow();

	void InitTimer();

	// Used to properly quit the game
	void Quit();

	// Updates the timer for this frame
	void UpdateTimer();

	// Calculates stats about the current frame and
	// updates the window's title bar
	void CalculateFrameStats();

public:
	LRESULT WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

private:

	void OnResize();

	// Convenience methods for handling mouse input, since we
	// can easily grab that information from OS-level messages
	void OnMouseDown(WPARAM btnState, int x, int y) { }
	void OnMouseUp(WPARAM btnState, int x, int y) { }
	void OnMouseMove(WPARAM btnState, int x, int y) { }

private:
	// Window handles and such
	HINSTANCE hAppInst;
	HWND      hMainWnd;

	// Game and window state tracking
	bool      hasFocus;
	bool      minimized;
	bool      maximized;
	bool      resizing;

	// Derived class can set these in derived constructor to customize starting values.
	std::wstring windowCaption;
	int windowWidth;
	int windowHeight;

	// Timer related data
	double perfCounterSeconds;
	__int64 startTime;
	__int64 currentTime;
	__int64 previousTime;
	float totalTime;
	float deltaTime;

	bool quit;

	std::function<void(int, int)>	callbackOnResize;
};