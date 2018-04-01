// ----------------------------------------------------------------------------
//  A few notes on project settings
//
//  - The project is set to use the UNICODE character set
//    - This was changed in Project Properties > Config Properties > General > Character Set
//    - This basically adds a "#define UNICODE" to the project
//
//  - The include directories were automagically correct, since the DirectX 
//    headers and libs are part of the windows SDK
//    - For instance, $(WindowsSDK_IncludePath) is set as a project include 
//      path by default.  That's where the DirectX headers are located.
//
//  - Two libraries had to be manually added to the Linker Input Dependencies
//    - d3d11.lib
//    - d3dcompiler.lib
//    - This was changed in Project Properties > Config Properties > Linker > Input > Additional Dependencies
//
//  - The Working Directory was changed to match the actual .exe's 
//    output directory, since we need to load the compiled shader files at run time
//    - This was changed in Project Properties > Config Properties > Debugging > Working Directory
//
// ----------------------------------------------------------------------------

//#define TEST_RENDERING_SYSTEM 1
#define TEST_NEW_ENGINE 1

#if defined(TEST_NEW_ENGINE)
 
#ifdef _DEBUG 
#ifndef DBG_NEW 
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ ) 
#define new DBG_NEW 
#endif 
#endif  // _DEBUG

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include <Windows.h>
#include "Engine.h"

void CreateConsoleWindow(int bufferLines, int bufferColumns, int windowLines, int windowColumns);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, LPSTR cmdLine, int showCmd)
{
	// Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	long lBreakAlloc = 7732;
	if (lBreakAlloc > 0)
	{
		_CrtSetBreakAlloc(lBreakAlloc);
	}

	// Allocate console in debug mode
	AllocConsole();
	// Redirect the CRT standard input, output, and error handles to the console
	freopen("CONIN$", "r", stdin);
	freopen("CONOUT$", "w", stdout);
	freopen("CONOUT$", "w", stderr);
#endif

	Engine engine;

	if (!engine.Init())
		return - 1;
	
#if defined(DEBUG) || defined(_DEBUG)
	// Do we want a console window?  Probably only in debug mode
	CreateConsoleWindow(500, 120, 32, 120);
	printf("Console window created successfully.  Feel free to printf() here.");
#endif

	return engine.Run();
}

void CreateConsoleWindow(int bufferLines, int bufferColumns, int windowLines, int windowColumns)
{
	// Our temp console info struct
	CONSOLE_SCREEN_BUFFER_INFO coninfo;

	// Get the console info and set the number of lines
	AllocConsole();
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &coninfo);
	coninfo.dwSize.Y = bufferLines;
	coninfo.dwSize.X = bufferColumns;
	SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), coninfo.dwSize);

	SMALL_RECT rect;
	rect.Left = 0;
	rect.Top = 0;
	rect.Right = windowColumns;
	rect.Bottom = windowLines;
	SetConsoleWindowInfo(GetStdHandle(STD_OUTPUT_HANDLE), TRUE, &rect);

	FILE *stream;
	freopen_s(&stream, "CONIN$", "r", stdin);
	freopen_s(&stream, "CONOUT$", "w", stdout);
	freopen_s(&stream, "CONOUT$", "w", stderr);

	// Prevent accidental console window close
	HWND consoleHandle = GetConsoleWindow();
	HMENU hmenu = GetSystemMenu(consoleHandle, FALSE);
	EnableMenuItem(hmenu, SC_CLOSE, MF_GRAYED);
}

#elif defined(TEST_RENDERING_SYSTEM)

// REMOVE THIS PART !!!!!

#include "NativeWindow.h"
#include "RenderingSystem.h"
#include "DebugCam.h"
#include "SceneGraph.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, LPSTR cmdLine, int showCmd)
{
	// Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	NativeWindow window;
	RenderingSystem renderer;

	if (!window.Init() || !renderer.Init(&window))
		return 0;

	SceneGraph scene;

	Renderable* r = scene.CreateRenderable();

	Shader* shader = renderer.CreateShader(L"Assets/ShaderObjs/Opaque.cso");

	Mesh* mesh = renderer.CreateMesh("Assets/Models/cube.fbx");
	Texture* tex = renderer.CreateTexture(L"Assets/Textures/crate.png");
	Material* mat = renderer.CreateMaterial(shader);

	if (!mat->SetTexture("texDiffuse", tex))
	{
		return -1;
	}

	r->SetMesh(mesh);
	r->SetMaterial(mat);

	DirectX::XMFLOAT4X4 matrix;
	DirectX::XMStoreFloat4x4(&matrix, DirectX::XMMatrixIdentity());

	mat->SetMatrix4x4("matWorld", matrix);
	mat->SetMatrix4x4("matWorld_IT", matrix);

	DebugCam cam;
	cam.getViewMatrix();
	cam.setProjectionMatrix(800.0f / 600.0f);
	
	float rot = 0.0f;
	while (!window.WindowIsClosed())
	{
		window.ProcessEvent();

		float deltaTime = window.GetDeltaTime();

		cam.update(deltaTime);

		rot += deltaTime * 1.0f;

		auto rotM = DirectX::XMMatrixRotationY(rot);

		DirectX::XMStoreFloat4x4(&matrix, DirectX::XMMatrixTranspose(rotM));
		mat->SetMatrix4x4("matWorld", matrix);
		DirectX::XMStoreFloat4x4(&matrix, DirectX::XMMatrixInverse(nullptr, rotM));
		mat->SetMatrix4x4("matWorld_IT", matrix);

		renderer.DrawScene(&cam, &scene);
	} 

	return 0;
}

#else

#include "Main.h"
#include "Vertex.h"

// For the DirectX Math library
using namespace DirectX;


#pragma region Win32 Entry Point (WinMain)
// --------------------------------------------------------
// Win32 Entry Point - Where your program starts
// --------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance,
	PSTR cmdLine, int showCmd)
{
	// Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	// Create the game object.
	Main game(hInstance);



	// This is where we'll create the window, initialize DirectX, 
	// set up geometry and shaders, etc.
	if (!game.Init())
		return 0;

	// All set to run the game loop
	return game.Run();
}

#pragma endregion

#pragma region Constructor / Destructor
// --------------------------------------------------------
// Base class constructor will set up all of the underlying
// fields, and then we can overwrite any that we'd like
// --------------------------------------------------------
Main::Main(HINSTANCE hInstance)
	: DirectXGameCore(hInstance)
{
	// Set up a custom caption for the game window.
	// - The "L" before the string signifies a "wide character" string
	// - "Wide" characters take up more space in memory (hence the name)
	// - This allows for an extended character set (more fancy letters/symbols)
	// - Lots of Windows functions want "wide characters", so we use the "L"
	windowCaption = L"DirectX Engine";

	// Custom window size - will be created by Init() later
	windowWidth = 800;
	windowHeight = 600;

	//initialize
	meshOne = nullptr;

	cam = new Camera(); 
	debugCam = new DebugCam();

	leftmouseHeld = false;
	middleMouseHeld = false;
	rightmouseHeld = false;


}

// --------------------------------------------------------
// Cleans up our DirectX stuff and any objects we need to delete
// - When you make new DX resources, you need to release them here
// - If you don't, you get a lot of scary looking messages in Visual Studio
// --------------------------------------------------------
Main::~Main()
{
	ReleaseMacro(sampBasic);

	// Release any D3D stuff that's still hanging out
	ReleaseMacro(vertexBuffer);
	ReleaseMacro(indexBuffer);
	ReleaseMacro(rasterizerState);
	ReleaseMacro(depthStencilState);
	ReleaseMacro(sampler);
	ReleaseMacro(skyBox);

	// Delete our simple shaders
	delete vertexShader;
	delete pixelShader;
	delete skyVertShader;
	delete skyPixShader;

	// Delete Meshes
	delete skyMesh;
	delete meshOne;

	//Delete Entities
	delete skyObject;
	for (int i = 0; i < MAX_ENTITIES; i++)
	{
		delete entities[i];
	}
	testEnt->RemoveAllComponents();
	delete testEnt;
	
	//Delete Material
	delete skyMaterial;
	delete material;

	//Delete Camera
	delete cam; 
	delete debugCam;
}

#pragma endregion

#pragma region Initialization

// --------------------------------------------------------
// Initializes the base class (including the window and D3D),
// sets up our geometry and loads the shaders (among other things)
// --------------------------------------------------------
bool Main::Init()
{
	// Call the base class's Init() method to create the window,
	// initialize DirectX, etc.
	if (!DirectXGameCore::Init())
		return false;

	// Helper methods to create something to draw, load shaders to draw it 
	// with and set up matrices so we can see how to pass data to the GPU.
	//  - For your own projects, feel free to expand/replace these.
	ComponentManager::NewComManager();
	LoadShaders();
	CreateGeometry();
	CreateMatrices();
	


	// Initialize Deferred Context
	//device->CreateDeferredContext(0, &deferredContext);

	//Reference: SkyMap retrieved from http://www.custommapmakers.org/skyboxes/zips/sor_borg.zip
	if (S_OK !=
		CreateDDSTextureFromFile(
			device,
			deviceContext,
			L"Assets/Textures/space.dds",
			0,
			&skyBox)
		)
	{
		return false;
	}

	// Create a sampler state for texture sampling
	D3D11_SAMPLER_DESC samplerDesc = {};
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	samplerDesc.MaxAnisotropy = 16;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	// Ask the device to create a state
	device->CreateSamplerState(&samplerDesc, &sampler);


	D3D11_DEPTH_STENCIL_DESC lessEqualsDesc = {};
	lessEqualsDesc.DepthEnable = true;
	lessEqualsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	lessEqualsDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

	device->CreateDepthStencilState(&lessEqualsDesc, &depthStencilState);

	// Set up a rasterizer state with no culling
	D3D11_RASTERIZER_DESC rd = {};
	rd.CullMode = D3D11_CULL_FRONT;
	rd.FillMode = D3D11_FILL_SOLID;
	rd.DepthClipEnable = true;
	device->CreateRasterizerState(&rd, &rasterizerState);

	// Tell the input assembler stage of the pipeline what kind of
	// geometric primitives we'll be using and how to interpret them
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Initialize Light Sources 

	// Directional Lights 
	directionalLight.AmbientColor = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
	directionalLight.DiffuseColor = XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);
	directionalLight.Direction = XMFLOAT3(-1.0f, -1.0f, 0.0f);
	pixelShader->SetData(
		"directionalLight",	//name in shader variable
		&directionalLight,	// address in memory
		sizeof(DirectionalLight)); // size of data to copy 


	directionalLight2.AmbientColor = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
	directionalLight2.DiffuseColor = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	directionalLight2.Direction = XMFLOAT3(0.0f, -1.0f, -1.0f);
	pixelShader->SetData(
		"directionalLight2",	//name in shader variable
		&directionalLight2,	// address in memory
		sizeof(DirectionalLight)); // size of data to copy 

	// Point Lights 
	pointLight.PointLightColor = XMFLOAT4(1.0f, 1.0f, 1.0f, 0.0f);
	pointLight.Position = XMFLOAT3(0.0f, 1.0f, -3.0f);
	pixelShader->SetData(
		"pointLight",	//name in shader variable
		&pointLight,	// address in memory
		sizeof(PointLight)); // size of data to copy 

	// Store Camera for for specular lighting 
#ifdef _DEBUG
	pixelShader->SetData("camPos", &debugCam->getPosition(), sizeof(XMFLOAT3));
#else
	pixelShader->SetData("camPos", &cam->getPosition(), sizeof(XMFLOAT3));
#endif // _DEBUG

	// Specular Lights 
	specularLight.SpecularColor = XMFLOAT4(1.0f, 0.1449275f, 0.0f, 1.0f);
	specularLight.Direction = XMFLOAT3(-3.0f, -1.0f, -2.0f);
	specularLight.SpecularStrength = 0.75f;
	specularLight.LightIntensity = 0.5f;
	pixelShader->SetData(
		"specularLight",	//name in shader variable
		&specularLight,	// address in memory
		sizeof(SpecularLight)); // size of data to copy 

	// Successfully initialized
	return true;
}


// --------------------------------------------------------
// Loads shaders from compiled shader object (.cso) files
// - These simple shaders provide helpful methods for sending
//   data to individual variables on the GPU
// --------------------------------------------------------
void Main::LoadShaders()
{
	vertexShader = new SimpleVertexShader(device, deviceContext);
	//vertexShader->LoadShaderFile(L"Assets/ShaderObjs/VertexShader.cso");
	vertexShader->LoadShaderFile(L"Assets/ShaderObjs/Opaque_VS.cso");

	pixelShader = new SimplePixelShader(device, deviceContext);
	//pixelShader->LoadShaderFile(L"Assets/ShaderObjs/PixelShader.cso");
	pixelShader->LoadShaderFile(L"Assets/ShaderObjs/Opaque_PS.cso");

	skyVertShader = new SimpleVertexShader(device, deviceContext);
	skyVertShader->LoadShaderFile(L"Assets/ShaderObjs/VertexShaderSky.cso");

	skyPixShader = new SimplePixelShader(device, deviceContext);
	skyPixShader->LoadShaderFile(L"Assets/ShaderObjs/PixelShaderSky.cso");
}

// --------------------------------------------------------
// Creates the geometry we're going to draw - a single triangle for now
// --------------------------------------------------------
void Main::CreateGeometry()
{
	skyMaterial = new Material(skyVertShader, skyPixShader);
	skyMesh = new Mesh("Assets/Models/cube.fbx", device);

	skyObject = new Entity(skyMesh, skyMaterial);
#ifdef _DEBUG
		skyObject->SetPosition(debugCam->getPosition().x, debugCam->getPosition().y, debugCam->getPosition().z);
#else
	skyObject->SetPosition(cam->getPosition().x, cam->getPosition().y, cam->getPosition().z);
#endif // _DEBUG

	skyObject->SetScale(200.0f, 200.0f, 200.0f);

	//	Generic UVs
	XMFLOAT3 normal = XMFLOAT3(0, 0, -1);
	XMFLOAT2 uv = XMFLOAT2(0, 0);

	// Create some temporary variables to represent colors
	// - Not necessary, just makes things more readable
	XMFLOAT4 red = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	XMFLOAT4 green = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
	XMFLOAT4 blue = XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);

	//meshOne = new Mesh(vertices, (int)sizeof(vertices), indices, sizeof(indices), device);
	meshOne = new Mesh("Assets/Models/cube.fbx", device);

	//Create Material 
	material = new Material(vertexShader, pixelShader);

	texDiffuse.LoadTextureFromFile(L"Assets/Textures/crate.png", device);
	material->texDiffuse = &texDiffuse;

	D3D11_SAMPLER_DESC sampDesc = {};
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	HR(device->CreateSamplerState(&sampDesc, &sampBasic));

	material->sampBasic = sampBasic;

	//Create entities 
	//Entity* e1 = new Entity(meshOne, material);
	//Entity* e2 = new Entity(meshThree, material);
	//Entity* e3 = new Entity(meshTwo, material);


	// Organize fixed amount of entities in array 
	for (int i = 0; i < MAX_ENTITIES; ++i)
	{
		entities[i] = new Entity(meshOne, material);
		// make entities tiny
		entities[i]->SetScale(0.25f, 0.25f, 0.25f);
	}
	float xPos = 0.0f;
	float yPos = 0.0f;
	for (int i = 0; i < MAX_ENTITIES; ++i)
	{
		if (i % 5 == 0)
		{
			xPos = 0.0f;
			yPos -= 0.75f;
			entities[i]->Move(xPos, yPos, 0);
		}
		else
		{
			xPos += 0.75f;
			entities[i]->Move(xPos, yPos, 0);
		}

	}
	testEnt = new Entity(meshOne, material);
	testEnt->SetScale(2, 2, 2);
	testEnt->SetPosition(0, 0, 0);
	testEnt->AddComponent<ScriptComponent>("script.lua");
}


// --------------------------------------------------------
// Initializes the matrices necessary to represent our geometry's 
// transformations and our 3D camera
// --------------------------------------------------------
void Main::CreateMatrices()
{
	// Set up world matrix
	// - In an actual game, each object will need one of these and they should
	//   update when/if the object moves (every frame)
	XMMATRIX W = XMMatrixIdentity();
	XMStoreFloat4x4(&worldMatrix, XMMatrixTranspose(W)); // Transpose for HLSL!

	// Create the View matrix
	// - In an actual game, recreate this matrix when the camera 
	//    moves (potentially every frame)
	// - We're using the LOOK TO function, which takes the position of the
	//    camera and the direction you want it to look (as well as "up")
	// - Another option is the LOOK AT function, to look towards a specific
	//    point in 3D space
	XMVECTOR pos = XMVectorSet(0, 0, -5, 0);
	XMVECTOR dir = XMVectorSet(0, 0, 1, 0);
	XMVECTOR up = XMVectorSet(0, 1, 0, 0);
	XMMATRIX V = XMMatrixLookToLH(
		pos,     // The position of the "camera"
		dir,     // Direction the camera is looking
		up);     // "Up" direction in 3D space (prevents roll)
	XMStoreFloat4x4(&viewMatrix, XMMatrixTranspose(V)); // Transpose for HLSL!

	// Create the Projection matrix
	// - This should match the window's aspect ratio, and also update anytime
	//   the window resizes (which is already happening in OnResize() below)
	XMMATRIX P = XMMatrixPerspectiveFovLH(
		0.25f * 3.1415926535f,		// Field of View Angle
		aspectRatio,				// Aspect ratio
		0.1f,						// Near clip plane distance
		100.0f);					// Far clip plane distance
	XMStoreFloat4x4(&projectionMatrix, XMMatrixTranspose(P)); // Transpose for HLSL!

#ifdef _DEBUG
	debugCam->setProjectionMatrix(aspectRatio);
#endif // _DEBUG

}

#pragma endregion

#pragma region Window Resizing

// --------------------------------------------------------
// Handles resizing DirectX "stuff" to match the (usually) new
// window size and updating our projection matrix to match
// --------------------------------------------------------
void Main::OnResize()
{
	// Handle base-level DX resize stuff
	DirectXGameCore::OnResize();

#ifdef _DEBUG
	debugCam->setProjectionMatrix(aspectRatio);
#else
	cam->onResize(aspectRatio);
#endif // _DEBUG
}
#pragma endregion

#pragma region Engine Loop

// --------------------------------------------------------
// Update your game here - take input, move objects, etc.
// --------------------------------------------------------
void Main::UpdateScene(float deltaTime, float totalTime)
{
	//Skymap position
#ifdef _DEBUG
	skyObject->SetPosition(debugCam->getPosition().x, debugCam->getPosition().y, debugCam->getPosition().z);
#else
	skyObject->SetPosition(cam->getPosition().x, cam->getPosition().y, cam->getPosition().z);
#endif // _DEBUG


	// Update User Input
	InputManager::instance().UpdateInput(deltaTime);

	// Quit if the escape key is pressed
	if (InputManager::instance().GetQuit())
		Quit();


	// arbitrary constants that control movement  
	float speed = 0.25f * deltaTime;
	float rotation = 0.55f * deltaTime;
	float buffer = 1.5f;

	// Manipulate matrices
	for (auto& i : entities)
	{
		i->Rotate(0, rotation, 0);
	}

	// Input
	//if (leftmouseHeld) { entities[0]->Move(speed, 0, 0); }
	//if (middlemouseHeld) { entities[1]->Move(speed, 0, 0); }
	//if (rightmouseHeld) { entities[2]->Move(speed, 0, 0); }

	//update all entities 
	for (auto& i : entities)
	{
		i->updateScene();
	}

	//update Camera and it's input
#ifdef _DEBUG
	debugCam->update(deltaTime, totalTime);
#else
	cam->cameraInput(deltaTime);
	cam->update(deltaTime);
 

	// Testing ------------------------------------------------------------------------

#endif // _DEBUG

	//InputManager::instance().GetA(); 
	if (GetAsyncKeyState(VK_LEFT) & 0x8000)
	{ 
		if (testEnt->AddComponent<ScriptComponent>("script.lua")) {
			//cout << "yes" << endl;
		}
		else {
			//cout << "no" << endl;
		}
	}
	if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
	{
		if (testEnt->RemoveComponent<ScriptComponent>()) {
			//cout << "yes" << endl;
		}
		else {
			//cout << "no" << endl;
		}
	}
}

// --------------------------------------------------------
// Clear the screen, redraw everything, present to the user
// --------------------------------------------------------
void Main::DrawScene(float deltaTime, float totalTime)
{
	// Background color (Cornflower Blue in this case) for clearing
	const float color[4] = { 0.4f, 0.6f, 0.75f, 0.0f };

	// Clear the render target and depth buffer (erases what's on the screen)
	//  - Do this ONCE PER FRAME
	//  - At the beginning of DrawScene (before drawing *anything*)
	deviceContext->ClearRenderTargetView(renderTargetView, color);
	deviceContext->ClearDepthStencilView(
		depthStencilView,
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f,
		0);

	deviceContext->RSSetState(nullptr);

	// Set the vertex and pixel shaders to use for the next Draw() command
	//  - These don't technically need to be set every frame...YET
	//  - Once you start applying different shaders to different objects,
	//    you'll need to swap the current shaders before each draw
	vertexShader->SetShader(true);
	pixelShader->SetShader(true);

	for (auto& i : entities)
	{
		// Send data to shader variables
		//  - Do this ONCE PER OBJECT you're drawing
		//  - This is actually a complex process of copying data to a local buffer
		//    and then copying that entire buffer to the GPU.  
		//  - The "SimpleShader" class handles all of that for you.
#ifdef _DEBUG

		i->prepareMaterial(debugCam->getViewMatrix(), debugCam->getProjectionMatrix());
#else
		i->prepareMaterial(cam->getViewMatrix(), cam->getProjectionMatrix());
#endif // _DEBUG

		//draw here 
		i->drawScene(deviceContext);


		//i->drawDeferred(deferredContext, commandList);

		// Wait for completion of command list
		//deferredContext->FinishCommandList(FALSE, &commandList);

		//Execute deferred commands
		//deviceContext->ExecuteCommandList(commandList, FALSE);
	}

	testEnt->prepareMaterial(cam->getViewMatrix(), cam->getProjectionMatrix());
	testEnt->drawScene(deviceContext);
	//Execute deferred commands
	//deviceContext->ExecuteCommandList(commandList, FALSE);

	// Draw SkyBox

	deviceContext->RSSetState(rasterizerState);
	deviceContext->OMSetDepthStencilState(depthStencilState, 1);

#ifdef _DEBUG
	skyObject->prepareMaterial(debugCam->getViewMatrix(), debugCam->getProjectionMatrix());
#else
	skyObject->prepareMaterial(cam->getViewMatrix(), cam->getProjectionMatrix());
#endif // _DEBUG

	
	skyPixShader->SetSamplerState("linearSampler", sampler);
	skyPixShader->SetShaderResourceView("skyMap", skyBox);
	skyObject->drawScene(deviceContext);

	skyVertShader->SetShader();
	skyPixShader->SetShader();

	// Present the buffer
	//  - Puts the image we're drawing into the window so the user can see it
	//  - Do this exactly ONCE PER FRAME
	//  - Always at the very end of the frame
	HR(swapChain->Present(0, 0));
}



#pragma endregion

#pragma region Mouse Input

// --------------------------------------------------------
// Helper method for mouse clicking.  We get this information
// from the OS-level messages anyway, so these helpers have
// been created to provide basic mouse input if you want it.
//
// Feel free to add code to this method
// --------------------------------------------------------
void Main::OnMouseDown(WPARAM btnState, int x, int y)
{
	// Save the previous mouse position, so we have it for the future
	prevMousePos.x = x;
	prevMousePos.y = y;

	//mouse input
	if (btnState & 0x0001) { /* Left button is down */ leftmouseHeld = true; }
	else { leftmouseHeld = false; }
	if (btnState & 0x0002) { /* Right button is down */ rightmouseHeld = true; }
	else { rightmouseHeld = false; }
	if (btnState & 0x0010) { /* Middle button is down */ middleMouseHeld = true; }
	else { middleMouseHeld = false; }

	// Pass values to Input Manager
	InputManager::instance().SetLeftMouseHeld(leftmouseHeld);
	InputManager::instance().SetRightMouseHeld(rightmouseHeld);
	InputManager::instance().SetMiddleMouseHeld(middleMouseHeld);

	// Caputure the mouse so we keep getting mouse move
	// events even if the mouse leaves the window.  we'll be
	// releasing the capture once a mouse button is released
	SetCapture(hMainWnd);
}

// --------------------------------------------------------
// Helper method for mouse release
//
// Feel free to add code to this method
// --------------------------------------------------------
void Main::OnMouseUp(WPARAM btnState, int x, int y)
{
	// We don't care about the tracking the cursor outside
	// the window anymore (we're not dragging if the mouse is up)
	ReleaseCapture();
}

// --------------------------------------------------------
// Helper method for mouse movement.  We only get this message
// if the mouse is currently over the window, or if we're 
// currently capturing the mouse.
//
// Feel free to add code to this method
// --------------------------------------------------------
void Main::OnMouseMove(WPARAM btnState, int x, int y)
{
#ifdef _DEBUG
	if (btnState & 0x0001)	// Left button is down
	{
		debugCam->moveSideways((x - prevMousePos.x) * 0.005f);
		debugCam->moveVertical((y - prevMousePos.y) * 0.005f);
	}
	if (btnState & 0x0002)	// Right button is down
	{
		debugCam->setRotationY(x - prevMousePos.x);
		debugCam->setRotationX(y - prevMousePos.y);
	}
#else
	//calc cam coords
	float camX = x - ((float)prevMousePos.x);
	float camY = y - ((float)prevMousePos.y);

	if (!InputManager::instance().GetGamePadEnabled())
	{
		cam->turnWithMouse(camX, camY);
	}
#endif // _DEBUG

	// Save the previous mouse position, so we have it for the future
	prevMousePos.x = x;
	prevMousePos.y = y;
	
	 
}
void Main::OnMouseWheel(float wheelDelta, int x, int y)
{
#ifdef _DEBUG
	debugCam->moveAlongDirection(wheelDelta * 0.1f);
#endif // _DEBUG

}
#pragma endregion

#endif
