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

	void* GetWindowHandle() const { return (void*)hMainWnd; }

	bool WindowIsClosed() const { return quit; }

	void SetResizeCallback(const std::function<void(int, int)>& func) { callbackOnResize = func; }

	// Calculates stats about the current frame and
	// updates the window's title bar
	void CalculateFrameStats(float totalTime);

private:
	// Handles window initialization
	bool InitMainWindow();

	// Used to properly quit the game
	void Quit();

public:
	LRESULT WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

private:

	void OnResize();

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

	bool quit;

	std::function<void(int, int)>	callbackOnResize;
};