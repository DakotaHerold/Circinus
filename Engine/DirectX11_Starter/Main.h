#pragma once

#include <DirectXMath.h>
#include <thread>
#include <mutex>
#include "DirectXGameCore.h"
#include "SimpleShader.h"
#include "Texture.h"
#include "Material.h"
#include "Mesh.h"
#include "Entity.h"
#include "Camera.h"
#include "Lights.h"
#include "InputManager.h"
#include "vld.h"
#include "DDSTextureLoader.h"


// Include run-time memory checking in debug builds, so 
// we can be notified of memory leaks
#if defined(DEBUG) || defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif

// --------------------------------------------------------
// Game class which extends the base DirectXGameCore class
// --------------------------------------------------------
class Main : public DirectXGameCore
{
public:
	Main(HINSTANCE hInstance);
	~Main();

	// Overrides for base level methods
	bool Init();
	void OnResize();
	void UpdateScene(float deltaTime, float totalTime);
	void DrawScene(float deltaTime, float totalTime);

	// For handing mouse input
	void OnMouseDown(WPARAM btnState, int x, int y);
	void OnMouseUp(WPARAM btnState, int x, int y);
	void OnMouseMove(WPARAM btnState, int x, int y);

private:
	// Initialization for our "game" demo - Feel free to
	// expand, alter, rename or remove these once you
	// start doing something more advanced!
	void LoadShaders(); 
	void CreateGeometry();
	void CreateMatrices();

	//DirectX related Buffers (for SkyBox)
	ID3D11RasterizerState* rasterizerState;
	ID3D11DepthStencilState* depthStencilState;

	//Sampler
	ID3D11SamplerState* sampler;

	//SkyBox
	ID3D11ShaderResourceView* skyBox;

	//Meshes
	Mesh* skyMesh;
	Mesh* meshOne;

	//Entities 
	Entity* skyObject;
	int MAX_ENTITIES = 100; 
	Entity* entities[100]; 

	//Camera
	Camera* cam; 

	//Material 
	Material* skyMaterial;
	Material* material; 

	//Lights
	DirectionalLight directionalLight; 
	DirectionalLight directionalLight2;
	SpecularLight specularLight; 
	PointLight pointLight; 

	//Misc
	bool leftmouseHeld; 
	bool middleMouseHeld;
	bool rightmouseHeld;

	// Deferred Rendering
	ID3D11DeviceContext* deferredContext = NULL;
	ID3D11CommandList* commandList = NULL;

	// Buffers to hold actual geometry data
	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;

	// Wrappers for DirectX shaders to provide simplified functionality
	SimpleVertexShader* vertexShader;
	SimplePixelShader* pixelShader;
	SimpleVertexShader* skyVertShader;
	SimplePixelShader* skyPixShader;

	Texture texDiffuse;
	ID3D11SamplerState*	sampBasic;

	// The matrices to go from model space to screen space
	DirectX::XMFLOAT4X4 worldMatrix;
	DirectX::XMFLOAT4X4 viewMatrix;
	DirectX::XMFLOAT4X4 projectionMatrix;

	// Keeps track of the old mouse position.  Useful for 
	// determining how far the mouse moved in a single frame.
	POINT prevMousePos;
};