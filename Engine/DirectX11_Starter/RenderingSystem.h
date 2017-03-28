#pragma once

#include <Windows.h>
#include <string>
#include <d3d11.h>

#include "dxerr.h"

// --------------------------------------------------------
// Convenience macro for releasing COM objects.
//
// Any time you get a reference from the DirectX API, you
// must release that reference.  This macro simplifies that.
// --------------------------------------------------------
#define ReleaseMacro(x) { if(x){ x->Release(); x = 0; } }

// --------------------------------------------------------
// Macro for checking the result of a DirectX function call.  This will
// pop up a message box on a failed result and then quit.  This macro
// depends on the "dxerr" (DirectX Error) helper files.
//
// In release mode this macro effectively does nothing.
// --------------------------------------------------------
#if defined(DEBUG) | defined(_DEBUG)
#ifndef HR
#define HR(x)												\
	{															\
		HRESULT hr = (x);										\
		if(FAILED(hr))											\
		{														\
			DXTrace(__FILEW__, (DWORD)__LINE__, hr, L#x, true);	\
			PostQuitMessage(0);									\
		}														\
	}														
#endif
#else
#ifndef HR
#define HR(x) (x) // Do nothing special!
#endif
#endif

class SceneGraph;

class RenderingSystem
{
public:
	RenderingSystem();
	~RenderingSystem(void);

	// Methods called by the game loop - override these in
	// derived classes to implement custom functionality
	bool Init(void* wndHandle);
	void OnResize(int windowWidth, int windowHeight);
	void DrawScene(SceneGraph* scene);

protected:
	// Handles Direct3D initialization
	bool InitDirect3D(void* wndHandle);

	// DirectX related buffers, views, etc.
	ID3D11Device*             device;
	ID3D11DeviceContext*      deviceContext;
	IDXGISwapChain*           swapChain;
	ID3D11Texture2D*          depthStencilBuffer;
	ID3D11RenderTargetView*   renderTargetView;
	ID3D11DepthStencilView*   depthStencilView;
	D3D11_VIEWPORT            viewport;
	D3D_DRIVER_TYPE           driverType;
	D3D_FEATURE_LEVEL         featureLevel;

	// The window's aspect ratio, used mostly for your projection matrix
	float aspectRatio;

	// Derived class can set these in derived constructor to customize starting values.
	int windowWidth;
	int windowHeight;
};

