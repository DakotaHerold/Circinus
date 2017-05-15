#pragma once

#include "Config.h"

#include <Windows.h>
#include <string>
#include <vector>
#include <unordered_map>
#include <d3d11.h>

#include "dxerr.h"

#include "ConstantBuffer.h"
#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"
#include "Material.h"
#include "Lights.h"

#ifdef HAS_GUI
#pragma region Include GUI

#include "GUI.h"

#pragma endregion
#endif

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

class NativeWindow;
class SceneGraph;
class ParticleSystem;
class GizmoRenderer;
class Camera;

class RenderingSystem
{
private:
	static RenderingSystem* _instance;

public:
	inline static RenderingSystem* instance() { return _instance; }

public:
	RenderingSystem();
	~RenderingSystem(void);

	bool Init(NativeWindow* win);

	void OnResize(int windowWidth, int windowHeight);

	void Update(float deltaTime, float totalTime);

	void DrawScene(Camera* cam, SceneGraph* scene);

public:

	int GetWindowWidth() const { return windowWidth; }
	int GetWindowHeight() const { return windowHeight; }

public:
	Mesh* CreateMesh(const wchar_t* filename);

	Shader* CreateShader(const wchar_t* filename);

	Texture* CreateTexture(const wchar_t* filename);

	Material* CreateMaterial(Shader* shader);

private:
	std::unordered_map<std::wstring, Mesh*>		meshes;
	std::unordered_map<std::wstring, Shader*>	shaders;
	std::unordered_map<std::wstring, Texture*>	textures;
	std::vector<Material*>						materials;

private:
	bool InitPreBoundConstantBuffers();

	void UploadPreBoundConstantBuffers();

	void UpdateViewMatrix(const DirectX::XMFLOAT4X4& m);

	void UpdateProjectionMatrix(const DirectX::XMFLOAT4X4& m);

	void UpdateCameraPosition(const DirectX::XMFLOAT3& f);

	void UpdateLightProperties(const DirectX::XMFLOAT4 & fe, const DirectX::XMFLOAT4 & fa, const Light* l, const int & n, const int & MAX_n);

private:

	struct BuiltinFrameCB
	{
		DirectX::XMFLOAT4X4		matView;
		DirectX::XMFLOAT4X4		matProj;
		DirectX::XMFLOAT3		camPos;
	};

	// TODO
	/*struct BuiltinInstanceCB
	{

	};*/

	ConstantBuffer			builtinFrameCB;
	ConstantBuffer			lightProperties;
	//BuiltinInstanceCB		builtinInstanceCB;

	std::unordered_map<std::string, ConstantBuffer*> preBoundCBs;

private:
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

	ParticleSystem*           particleSystem;

#ifdef HAS_EDITOR
	GizmoRenderer*            gizmoRenderer;
#endif

	// The window's aspect ratio, used mostly for your projection matrix
	float aspectRatio;
	bool aspectRatioChanged;

	// Derived class can set these in derived constructor to customize starting values.
	int windowWidth;
	int windowHeight;
};

