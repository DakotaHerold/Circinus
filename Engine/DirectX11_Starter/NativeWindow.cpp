#include "NativeWindow.h"

#include <WindowsX.h>
#include <sstream>

namespace
{
	// --------------------------------------------------------
	// The global callback function for handling windows OS-level messages.
	//
	// This needs to be a global function (not part of a class), but we want
	// to forward the parameters to our class to properly handle them.
	// --------------------------------------------------------
	LRESULT CALLBACK MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		if (msg == WM_DESTROY)
		{
			PostQuitMessage(0);
			return DefWindowProc(hwnd, msg, wParam, lParam);
		}

		LONG ptr = GetWindowLongPtr(hwnd, 0);

		if (!ptr)
			return DefWindowProc(hwnd, msg, wParam, lParam);

		NativeWindow* wnd = reinterpret_cast<NativeWindow*>(ptr);

		// Forward the global callback params to our game's message handler
		return wnd->WndProc(hwnd, msg, wParam, lParam);
	}
}

NativeWindow::NativeWindow()
	:
	hAppInst(0),
	windowCaption(L"DirectX Game"),
	windowWidth(800),
	windowHeight(600),
	hMainWnd(0),
	hasFocus(false),
	minimized(false),
	maximized(false),
	resizing(false),
	perfCounterSeconds(0.0),
	startTime(0),
	currentTime(0),
	previousTime(0),
	totalTime(0.0f),
	deltaTime(0.0f),
	quit(false)
{
	hAppInst = GetModuleHandle(nullptr);

	// Query the performance counter for accurate timing information
	__int64 perfFreq;
	QueryPerformanceFrequency((LARGE_INTEGER*)&perfFreq);
	perfCounterSeconds = 1.0 / (double)perfFreq;
}

bool NativeWindow::Init()
{
	// Create the actual window itself (no DirectX yet)
	if (!InitMainWindow())
		return false;

	return true;
}

int NativeWindow::ProcessEvent()
{
	// Create a variable to hold the current message
	MSG msg = { 0 };

	// Peek at the next message (and remove it from the queue)
	while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
	{
		// Handle this message
		TranslateMessage(&msg);
		DispatchMessage(&msg);

		if (msg.message == WM_QUIT)
		{
			quit = true;
			return (int)msg.wParam;
		}
	}

	// Update the timer for this frame
	UpdateTimer();

	// Standard game loop type stuff
	CalculateFrameStats();

	// If we make it outside the game loop, return the most
	// recent message's exit code
	return (int)msg.wParam;
}

bool NativeWindow::InitMainWindow()
{
	// We fill out a a big struct with the basic
	// information about the window we'd like
	WNDCLASS wc;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = MainWndProc;  // Can't be a member function!
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hAppInst;
	wc.hIcon = LoadIcon(0, IDI_APPLICATION);
	wc.hCursor = LoadCursor(0, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
	wc.lpszMenuName = 0;
	wc.lpszClassName = L"D3DWndClassName";

	// Register the window class struct, which must be done before
	// we attempt to create the window itself
	if (!RegisterClass(&wc))
	{
		MessageBox(0, L"RegisterClass Failed.", 0, 0);
		return false;
	}

	// We want the specified width and height to be the viewable client
	// area (not counting borders), so we need to adjust for borders
	RECT R = { 0, 0, windowWidth, windowHeight };
	AdjustWindowRect(&R, WS_OVERLAPPEDWINDOW, false);
	int realWidth = R.right - R.left;
	int realHeight = R.bottom - R.top;

	// Actually create the window the user will eventually see
	hMainWnd = CreateWindow(
		L"D3DWndClassName",
		windowCaption.c_str(),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		realWidth,
		realHeight,
		0,
		0,
		hAppInst,
		this);

	// Ensure the window was created properly
	if (!hMainWnd)
	{
		DWORD err = GetLastError();
		MessageBox(0, L"CreateWindow Failed.", 0, 0);
		return false;
	}

	// Finally show the window to the user
	ShowWindow(hMainWnd, SW_SHOW);
	return true;
}

void NativeWindow::InitTimer()
{
	// Grab the start time
	__int64 now;
	QueryPerformanceCounter((LARGE_INTEGER*)&now);
	startTime = now;
	currentTime = now;
	previousTime = now;
}

void NativeWindow::Quit()
{
	PostQuitMessage(0);
}

void NativeWindow::UpdateTimer()
{

	// Grab the current time
	__int64 now;
	QueryPerformanceCounter((LARGE_INTEGER*)&now);
	currentTime = now;

	// Calculate delta time and clamp to zero if negative.
	//  - This could happen if the processor goes into a
	//    power save mode or the process itself gets moved
	//    to another processor core.
	deltaTime = (float)((currentTime - previousTime) * perfCounterSeconds);
	if (deltaTime < 0.0f)
		deltaTime = 0.0f;

	// Calculate total time
	totalTime = (float)((currentTime - startTime) * perfCounterSeconds);

	// Save the previous time
	previousTime = currentTime;
}

void NativeWindow::CalculateFrameStats()
{
	static int frameCount = 0;
	static float timeElapsed = 0.0f;

	frameCount++;

	// Compute averages over ONE SECOND.
	if ((totalTime - timeElapsed) >= 1.0f)
	{
		float fps = (float)frameCount; // Count over one second
		float mspf = 1000.0f / fps;    // Milliseconds per frame

									   // Quick and dirty string manipulation for title bar text
		std::wostringstream outs;
		outs.precision(6);
		outs << windowCaption << L"    "
			<< L"Width: " << windowWidth << L"    "
			<< L"Height: " << windowHeight << L"    "
			<< L"FPS: " << fps << L"    "
			<< L"Frame Time: " << mspf << L"ms";

		//// Include feature level
		//switch (featureLevel)
		//{
		//case D3D_FEATURE_LEVEL_11_1: outs << "    DX 11.1"; break;
		//case D3D_FEATURE_LEVEL_11_0: outs << "    DX 11.0"; break;
		//case D3D_FEATURE_LEVEL_10_1: outs << "    DX 10.1"; break;
		//case D3D_FEATURE_LEVEL_10_0: outs << "    DX 10.0"; break;
		//case D3D_FEATURE_LEVEL_9_3:  outs << "    DX 9.3";  break;
		//case D3D_FEATURE_LEVEL_9_2:  outs << "    DX 9.2";  break;
		//case D3D_FEATURE_LEVEL_9_1:  outs << "    DX 9.1";  break;
		//default:                     outs << "    DX ???";  break;
		//}

		SetWindowText(hMainWnd, outs.str().c_str());

		// Reset frame count, and adjust time elapsed
		// to wait another second
		frameCount = 0;
		timeElapsed += 1.0f;
	}
}

LRESULT NativeWindow::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		// WM_ACTIVATE is sent when the window is activated or deactivated.
		// We're using this to keep track of focus, if that's useful to you.
	case WM_ACTIVATE:
		if (LOWORD(wParam) == WA_INACTIVE)
		{
			hasFocus = false;
		}
		else
		{
			hasFocus = true;
		}
		return 0;

		// WM_SIZE is sent when the user resizes the window.  
	case WM_SIZE:
		// Save the new client area dimensions.
		windowWidth = LOWORD(lParam);
		windowHeight = HIWORD(lParam);

		if (wParam == SIZE_MINIMIZED)
		{
			hasFocus = false;
			minimized = true;
			maximized = false;
		}
		else if (wParam == SIZE_MAXIMIZED)
		{
			hasFocus = true;
			minimized = false;
			maximized = true;
			OnResize();
		}
		else if (wParam == SIZE_RESTORED)
		{
			// Restoring from minimized state?
			if (minimized)
			{
				hasFocus = true;
				minimized = false;
				OnResize();
			}

			// Restoring from maximized state?
			else if (maximized)
			{
				hasFocus = true;
				maximized = false;
				OnResize();
			}
			else if (resizing)
			{
				// If user is dragging the resize bars, we do not resize 
				// the buffers here because as the user continuously 
				// drags the resize bars, a stream of WM_SIZE messages are
				// sent to the window, and it would be pointless (and slow)
				// to resize for each WM_SIZE message received from dragging
				// the resize bars.  So instead, we reset after the user is 
				// done resizing the window and releases the resize bars, which 
				// sends a WM_EXITSIZEMOVE message.
			}
			else // API call such as SetWindowPos or mSwapChain->SetFullscreenState.
			{
				OnResize();
			}
		}

		return 0;

		// WM_EXITSIZEMOVE is sent when the user grabs the resize bars.
	case WM_ENTERSIZEMOVE:
		resizing = true;
		return 0;

		// WM_EXITSIZEMOVE is sent when the user releases the resize bars.
		// Here we reset everything based on the new window dimensions.
	case WM_EXITSIZEMOVE:
		resizing = false;
		OnResize();
		return 0;

		// WM_DESTROY is sent when the window is being destroyed.
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

		// The WM_MENUCHAR message is sent when a menu is active and the user presses 
		// a key that does not correspond to any mnemonic or accelerator key. 
	case WM_MENUCHAR:
		// Don't beep when we alt-enter.
		return MAKELRESULT(0, MNC_CLOSE);

		// Catch this message so to prevent the window from becoming too small.
	case WM_GETMINMAXINFO:
		((MINMAXINFO*)lParam)->ptMinTrackSize.x = 200;
		((MINMAXINFO*)lParam)->ptMinTrackSize.y = 200;
		return 0;

		// Messages that correspond to mouse button being pressed while the cursor
		// is currently over our window
	case WM_LBUTTONDOWN:
	case WM_MBUTTONDOWN:
	case WM_RBUTTONDOWN:
		OnMouseDown(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		return 0;

		// Messages that correspond to mouse button being released while the cursor
		// is currently over our window
	case WM_LBUTTONUP:
	case WM_MBUTTONUP:
	case WM_RBUTTONUP:
		OnMouseUp(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		return 0;

		// Message that occurs while the mouse moves over the window or while
		// we're currently capturing it
	case WM_MOUSEMOVE:
		OnMouseMove(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		return 0;
	}

	// Some other message was sent, so call the default window procedure for it
	return DefWindowProc(hwnd, msg, wParam, lParam);
}

void NativeWindow::OnResize()
{
	if (callbackOnResize)
		callbackOnResize(windowWidth, windowHeight);
}
