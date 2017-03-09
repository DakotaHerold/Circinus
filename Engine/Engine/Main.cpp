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

	leftmouseHeld = false; 
	middlemouseHeld = false; 
	rightmouseHeld = false; 
}

// --------------------------------------------------------
// Cleans up our DirectX stuff and any objects we need to delete
// - When you make new DX resources, you need to release them here
// - If you don't, you get a lot of scary looking messages in Visual Studio
// --------------------------------------------------------
Main::~Main()
{
	// Release any D3D stuff that's still hanging out
	ReleaseMacro(vertexBuffer);
	ReleaseMacro(indexBuffer);

	// Delete our simple shaders
	delete vertexShader;
	delete pixelShader;

	// Delete Meshes
	delete meshOne;

	//Delete Entities
	for (int i = 0; i < MAX_ENTITIES; i++)
	{
		delete entities[i]; 
	}
	
	//Delete Material
	delete material;

	//Delete Camera
	delete cam; 
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

	LoadShaders();
	CreateGeometry();
	CreateMatrices();

	// Initialize Deferred Context
	//device->CreateDeferredContext(0, &deferredContext);

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
	pixelShader->SetData("camPos", &cam->getPosition(), sizeof(XMFLOAT3)); 

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
	mtx2.lock();
	vertexShader = new SimpleVertexShader(device, deviceContext);
	vertexShader->LoadShaderFile(L"VertexShader.cso");

	pixelShader = new SimplePixelShader(device, deviceContext);
	pixelShader->LoadShaderFile(L"PixelShader.cso");
	mtx2.unlock();
}


// --------------------------------------------------------
// Creates the geometry we're going to draw - a single triangle for now
// --------------------------------------------------------
void Main::CreateGeometry()
{
	mtx1.lock();
	//	Generic UVs
	XMFLOAT3 normal = XMFLOAT3(0, 0, -1); 
	XMFLOAT2 uv = XMFLOAT2(0, 0); 

	// Create some temporary variables to represent colors
	// - Not necessary, just makes things more readable
	XMFLOAT4 red = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	XMFLOAT4 green = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
	XMFLOAT4 blue = XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);


	//meshOne = new Mesh(vertices, (int)sizeof(vertices), indices, sizeof(indices), device);
	meshOne = new Mesh("Models/cube.obj", device); 

	//Create Material 
	material = new Material(vertexShader, pixelShader); 
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
			entities[i]->Move(xPos, yPos , 0);	
		}
		
	}
	mtx1.unlock();
}


// --------------------------------------------------------
// Initializes the matrices necessary to represent our geometry's 
// transformations and our 3D camera
// --------------------------------------------------------
void Main::CreateMatrices()
{
	mtx3.lock();
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
	mtx3.unlock();
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

	cam->onResize(aspectRatio); 
}
#pragma endregion

#pragma region Engine Loop

// --------------------------------------------------------
// Update your game here - take input, move objects, etc.
// --------------------------------------------------------
void Main::UpdateScene(float deltaTime, float totalTime)
{
	// Quit if the escape key is pressed
	if (GetAsyncKeyState(VK_ESCAPE))
		Quit();
	

	//constants that control movement  
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
	cam->cameraInput(deltaTime); 
	cam->update(deltaTime);
 
	//InputManager::instance().GetA(); 
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
		i->prepareMaterial(cam->getViewMatrix(), cam->getProjectionMatrix());
		//draw here 
		i->drawScene(deviceContext);
		//i->drawDeferred(deferredContext, commandList);

		// Wait for completion of command list
		//deferredContext->FinishCommandList(FALSE, &commandList);

		//Execute deferred commands
		//deviceContext->ExecuteCommandList(commandList, FALSE);
	}

	//Execute deferred commands
	//deviceContext->ExecuteCommandList(commandList, FALSE);
	


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
	if (btnState & 0x0001) { /* Left button is down */ leftmouseHeld = true; } else { leftmouseHeld = false;  }
	if (btnState & 0x0002) { /* Right button is down */ rightmouseHeld = true; } else { rightmouseHeld = false; }
	if (btnState & 0x0010) { /* Middle button is down */ middlemouseHeld = true; } else { middlemouseHeld = false; }
	


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
	//calc Cam coords
	float camX = x - ((float)prevMousePos.x);
	float camY = y - ((float)prevMousePos.y);
	// Save the previous mouse position, so we have it for the future
	prevMousePos.x = x;
	prevMousePos.y = y;
	

	cam->turn(camX, camY); 
}
#pragma endregion