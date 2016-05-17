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

#include "MyDemoGame.h"
#include "Vertex.h"
#include "btBulletCollisionCommon.h"
#include "btBulletDynamicsCommon.h"
#include "BulletCollision\Gimpact\btGImpactCollisionAlgorithm.h"
//#include "Entity.h"

// For the DirectX Math library
using namespace DirectX;



btBroadphaseInterface* MyDemoGame::broadphase = nullptr;

btDefaultCollisionConfiguration* MyDemoGame::collisionConfiguration = nullptr;
btCollisionDispatcher* MyDemoGame::dispatcher = nullptr;

btSequentialImpulseConstraintSolver* MyDemoGame::solver = nullptr;

btDiscreteDynamicsWorld* MyDemoGame::dynamicsWorld = nullptr;






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
	MyDemoGame game(hInstance);

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
MyDemoGame::MyDemoGame(HINSTANCE hInstance)
	: DirectXGameCore(hInstance)
{
	// Set up a custom caption for the game window.
	// - The "L" before the string signifies a "wide character" string
	// - "Wide" characters take up more space in memory (hence the name)
	// - This allows for an extended character set (more fancy letters/symbols)
	// - Lots of Windows functions want "wide characters", so we use the "L"
	windowCaption = L"My Super Fancy GGP Game";

	// Custom window size - will be created by Init() later
	windowWidth = 1280;
	windowHeight = 720;



	//initialize
	meshOne = nullptr;
	meshTwo = nullptr;
	meshThree = nullptr;

	enableDebugDraw = true; 

	e1 = nullptr;
	e2 = nullptr;
	e3 = nullptr;

	cam = new Camera();

	leftmouseHeld = false;
	middlemouseHeld = false;
	rightmouseHeld = false;

	//Input 
	pad = new GamePadXbox(GamePadIndex_One); 

	//Physics Initialization  
	collisionConfiguration = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(collisionConfiguration);
	//broadphase = new btDbvtBroadphase();
	btVector3 worldMin(-1000, -1000, -1000);
	btVector3 worldMax(1000, 1000, 1000);
	broadphase = new btAxisSweep3(worldMin, worldMax);
	solver = new btSequentialImpulseConstraintSolver();
	
	dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);
	dynamicsWorld->setGravity(btVector3(0.0f, -9.81f, 0.0f));
	//dynamicsWorld->performDiscreteCollisionDetection(); 

	
	
}

// --------------------------------------------------------
// Cleans up our DirectX stuff and any objects we need to delete
// - When you make new DX resources, you need to release them here
// - If you don't, you get a lot of scary looking messages in Visual Studio
// --------------------------------------------------------
MyDemoGame::~MyDemoGame()
{
	// Release any D3D stuff that's still hanging out
	ReleaseMacro(vertexBuffer);
	ReleaseMacro(indexBuffer);

	// Delete our simple shaders
	delete vertexShader;
	delete pixelShader;

	//delete debug shaders
	delete DrawDebugVertexShader; 
	delete DrawDebugPixelShader; 
	delete drawDebug; 
	//release
	//ReleaseMacro(DrawDebugVB); 

	// Delete Meshes
	delete meshOne;
	delete meshTwo;
	delete meshThree;

	//Delete Entities
	for (unsigned int i = 0; i < entities.size(); i++)
	{
		MyDemoGame::dynamicsWorld->removeRigidBody(entities[i]->collider);
		entities[i] = nullptr; 
		delete entities[i];
	}

	// Delete Physics World
	delete MyDemoGame::solver;
	delete MyDemoGame::collisionConfiguration;
	delete MyDemoGame::dispatcher;
	delete MyDemoGame::broadphase;
	delete MyDemoGame::dynamicsWorld;
	
	//Delete HUD
	for (unsigned int i = 0; i < UI.size(); i++)
	{
		delete UI[i];
	}
	 

	//Delete Material
	delete material;
	delete carMaterial; 
	delete ballMaterial; 

	//Delete Camera
	delete cam;

	//Delete Game Pad
	delete pad; 
}

#pragma endregion

#pragma region Initialization

// --------------------------------------------------------
// Initializes the base class (including the window and D3D),
// sets up our geometry and loads the shaders (among other things)
// --------------------------------------------------------
bool MyDemoGame::Init()
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

	// Tell the input assembler stage of the pipeline what kind of
	// geometric primitives we'll be using and how to interpret them
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Initialize Light Sources 

	// Directional Lights 
	directionalLight.AmbientColor = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
	directionalLight.DiffuseColor = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	directionalLight.Direction = XMFLOAT3(-1.0f, -1.0f, 0.0f);
	pixelShader->SetData(
		"directionalLight",	//name in shader variables
		&directionalLight,	// address in memory
		sizeof(DirectionalLight)); // size of data to copy 


	directionalLight2.AmbientColor = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
	directionalLight2.DiffuseColor = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	directionalLight2.Direction = XMFLOAT3(0.0f, -1.0f, 1.0f);
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
	specularLight.SpecularColor = XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f);
	specularLight.Direction = XMFLOAT3(1.0f, -1.0f, -5.0f);
	specularLight.SpecularStrength = 1.0f;
	specularLight.LightIntensity = 0.75f;
	pixelShader->SetData(
		"specularLight",	//name in shader variable
		&specularLight,	// address in memory
		sizeof(SpecularLight)); // size of data to copy 


								// Successfully initialized
	return true;
}


void MyDemoGame::UpdatePhysics(float deltaTime)
{

	
	// Update Physics 
	
	MyDemoGame::dynamicsWorld->stepSimulation(deltaTime, 10);

	for (int i = 0; i < entities.size(); i++)
	{
		entities[i]->CopyTransformFromBullet(); 
	}

	// Check Ball 
	btTransform trans;
	//entities[1]->motionState->getWorldTransform(trans);
 
	
	//Move vehicle 
	vehicle->applyEngineForce(engForce, 0);
	vehicle->setBrake(brakeForce, 0); 
	vehicle->applyEngineForce(engForce, 1);
	vehicle->setBrake(brakeForce, 1);
	vehicle->applyEngineForce(engForce, 2);
	vehicle->setBrake(brakeForce, 2);
	vehicle->applyEngineForce(engForce, 3);
	vehicle->setBrake(brakeForce, 3); 

	//Turn Vehicle - Front two wheels
	vehicle->setSteeringValue(steeringForce, 0); 
	vehicle->setSteeringValue(steeringForce, 1); 
	

	/*int numManifolds = dynamicsWorld->getDispatcher()->getNumManifolds(); 
	if (numManifolds > 0)
	{
		entities[1]->collider->activate(true); 
		entities[1]->collider->applyForce(btVector3(0, -20.0f, 0), btVector3(0,0,0)); 
	}*/
		
	// Check for Collisions 
	//entities[1]->collider->activate(true); 
	//entities[1]->collider->applyCentralImpulse(btVector3(0.f, 0.f, -0.005f)); 
	/*btVector3 floorPos = entities[0]->collider->getCenterOfMassPosition(); 
	if (entities[1]->collider->getCenterOfMassPosition().dot(floorPos) < 0.25f)
	{
		entities[1]->collider->activate(true);
		entities[1]->collider->applyCentralImpulse(btVector3(0.f, 20.f, 0.0f));
	}*/
}

// --------------------------------------------------------
// Loads shaders from compiled shader object (.cso) files
// - These simple shaders provide helpful methods for sending
//   data to individual variables on the GPU
// --------------------------------------------------------
void MyDemoGame::LoadShaders()
{
	vertexShader = new SimpleVertexShader(device, deviceContext);
	vertexShader->LoadShaderFile(L"VertexShader.cso");

	pixelShader = new SimplePixelShader(device, deviceContext);
	pixelShader->LoadShaderFile(L"PixelShader.cso");

	//Load Debug Shaders
	DrawDebugVertexShader = new SimpleVertexShader(device, deviceContext); 
	DrawDebugVertexShader->LoadShaderFile(L"DebugLineVS.cso"); 

	DrawDebugPixelShader = new SimplePixelShader(device, deviceContext);
	DrawDebugPixelShader->LoadShaderFile(L"DebugLinePS.cso");
}


// --------------------------------------------------------
// Creates the geometry we're going to draw - a single triangle for now
// --------------------------------------------------------
void MyDemoGame::CreateGeometry()
{
	//	Generic UVs
	XMFLOAT3 normal = XMFLOAT3(0, 0, -1);
	XMFLOAT2 uv = XMFLOAT2(0, 0);

	// Create some temporary variables to represent colors
	// - Not necessary, just makes things more readable
	XMFLOAT4 red = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	XMFLOAT4 green = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
	XMFLOAT4 blue = XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);

	// Set up the vertices of the triangle we would like to draw
	// - We're going to copy this array, exactly as it exists in memory
	//    over to a DirectX-controlled data structure (the vertex buffer)
	Vertex vertices[] =
	{
		{ XMFLOAT3(+0.0f, +1.0f, +0.0f), normal, uv },
		{ XMFLOAT3(+1.5f, -1.0f, +0.0f), normal, uv },
		{ XMFLOAT3(-1.5f, -1.0f, +0.0f), normal, uv },
	};

	// Set up the indices, which tell us which vertices to use and in which order
	// - This is somewhat redundant for just 3 vertices (it's a simple example)
	// - Indices are technically not required if the vertices are in the buffer 
	//    in the correct order and each one will be used exactly once
	// - But just to see how it's done...
	unsigned int indices[] = { 0, 1, 2 };


	//meshOne = new Mesh(vertices, (int)sizeof(vertices), indices, sizeof(indices), device);
	meshOne = new Mesh("Models/car_body.obj", device);


	//Create second Mesh
	Vertex triVerts[] =
	{
		{ XMFLOAT3(+0.0f, +2.0f, +0.0f), normal, uv },
		{ XMFLOAT3(+2.5f, -0.0f, +0.0f), normal, uv },
		{ XMFLOAT3(-0.5f, -0.0f, +0.0f), normal, uv },
	};
	unsigned int triIndices[] = { 0, 1, 2 };

	//meshTwo = new Mesh(triVerts, (int)sizeof(triVerts), triIndices, sizeof(triIndices), device);
	meshTwo = new Mesh("Models/sphere.obj", device);

	//Create third Mesh 

	Vertex triTwoVerts[] =
	{
		{ XMFLOAT3(+0.0f, -2.0f, +0.0f), normal, uv },
		{ XMFLOAT3(-2.0f, -0.0f, +0.0f), normal, uv },
		{ XMFLOAT3(+0.5f, -0.0f, +0.0f), normal, uv },
	};
	unsigned int triTwoIndices[] = { 0 , 1, 2 };

	//meshThree = new Mesh(triTwoVerts, (int)sizeof(triTwoVerts), triTwoIndices, sizeof(triTwoIndices), device);
	meshThree = new Mesh("Models/flatsurface.obj", device);

	meshFour = new Mesh("Models/car_wheel.obj", device);


	// Setup Physics
	// Collision Shapes 
	//btCollisionShape* shape1 = meshOne->triMesh; //cube
	//btCollisionShape* shape2 = meshTwo->triMesh; //sphere 
	/*btCollisionShape* shape1 = new btStaticPlaneShape(btVector3(0, 1, 0), 0);
	btCollisionShape* shape2 = new btSphereShape(1); */
	//btStaticPlaneShape* shape1 = new btStaticPlaneShape(btVector3(0, 1, 0), 0);
	btBoxShape* shape1 = new btBoxShape(btVector3(50, 0.01f, 50));
	btSphereShape* shape2 = new btSphereShape(1.0f);


	/*btTransform t; 
	t.setIdentity(); 
	t.setOrigin(btVector3(0, 0, 0)); */

	// Motion States and Rigid Bodies for collision shapes
	btDefaultMotionState* groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, -10.0f, 0)));
	btRigidBody::btRigidBodyConstructionInfo
		groundRigidBodyCI(0, groundMotionState, shape1, btVector3(0.0f, 0.0f, 0.0f));
	btRigidBody* groundRigidBody = new btRigidBody(groundRigidBodyCI);

	dynamicsWorld->addRigidBody(groundRigidBody); 
	 
	 


	btDefaultMotionState* fallMotionState =
		new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 20.0f, 0.0f)));
	btScalar mass = 1;
	btVector3 fallInertia(0.0f, 0.0f, 0.0f);
	shape2->calculateLocalInertia(mass, fallInertia);
	btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass, fallMotionState, shape2, fallInertia);
	fallRigidBodyCI.m_restitution = 0.5f;
	btRigidBody* fallRigidBody = new btRigidBody(fallRigidBodyCI);
	 
	dynamicsWorld->addRigidBody(fallRigidBody); 



	// BULLET VEHICLE -- reference to vehicle demo, and user ainurakne for a wheel problem -------------------------------------------------------------
	
	// make chassis with a collision shape and a rigid body, also added to dynamic world
	btScalar chassisMass(800.0f);
	//btScalar chassisMass(0.0f);
	btVector3 chassisInertia(0.0f, 0.0f, 0.0f);

	//btCollisionShape* chassisShape = new btBoxShape(btVector3(1.0f, .5f, 2.0f));
	btCollisionShape* chassisShape = new btBoxShape(btVector3(2.0f, .5f, 5.0f));
	btCompoundShape* compound = new btCompoundShape(); 
	btTransform localTrans; 
	localTrans.setIdentity(); 
	//localTrans.setOrigin(btVector3(0, 1, 0)); //collider location 
	localTrans.setOrigin(btVector3(0, 1.0f, 0)); //collider location 
	compound->addChildShape(localTrans, chassisShape); 

	btTransform tr; 
	tr.setIdentity(); 
	//tr.setOrigin(btVector3(0, -5.0f, 0)); //Controls where car spawns/renders 
	tr.setOrigin(btVector3(0, 0.0f, 0)); //Controls where car spawns/renders 
		// meshOne->conMesh;
		//new btBoxShape(btVector3(1.0f, .5f, 2.0f));

	//btDefaultMotionState* chassisMotionState = new btDefaultMotionState(btTransform(btQuaternion(0.0f, 0.0f, 0.0f, 1.0f), btVector3(0.0f, 5.0f, 0.0f)));
	btDefaultMotionState* chassisMotionState = new btDefaultMotionState(tr);

	chassisShape->calculateLocalInertia(chassisMass, chassisInertia);

	//btRigidBody::btRigidBodyConstructionInfo chassisRBCI(chassisMass, chassisMotionState, chassisShape, chassisInertia);
	btRigidBody::btRigidBodyConstructionInfo chassisRBCI(chassisMass, chassisMotionState, compound, chassisInertia);
	btRigidBody* chassisRB = new btRigidBody(chassisRBCI);
	//dynamicsWorld->addRigidBody(chassisRB);
	float wheelWidth = 0.4f; 
	float wheelRadius = 0.4f; 
	btCylinderShapeX* m_wheelShape = new btCylinderShapeX(btVector3(wheelWidth, wheelRadius, wheelRadius));

	//set world transform
	chassisRB->setWorldTransform(tr); 
	chassisRB->setContactProcessingThreshold(BT_LARGE_FLOAT); 

	//Initialize more values
	chassisRB->setCenterOfMassTransform(btTransform::getIdentity());
	chassisRB->setLinearVelocity(btVector3(0, 0, 0)); 
	chassisRB->setAngularVelocity(btVector3(0, 0, 0)); 
	dynamicsWorld->getBroadphase()->getOverlappingPairCache()->cleanProxyFromPairs(chassisRB->getBroadphaseHandle(), dynamicsWorld->getDispatcher()); 
	//vehicle->resetSuspension(); 

	/*for (int i = 0; i < vehicle->getNumWheels(); i++)
	{
		vehicle->updateWheelTransform(i, true); 
	}*/

	// setting raycasting for wheels and making vehicle
	btRaycastVehicle::btVehicleTuning tune;
	btVehicleRaycaster* caster = new btDefaultVehicleRaycaster(dynamicsWorld);
	vehicle = new btRaycastVehicle(tune, chassisRB, caster);
	chassisRB->setActivationState(DISABLE_DEACTIVATION);
	dynamicsWorld->addRigidBody(vehicle->getRigidBody()); 
	dynamicsWorld->addVehicle(vehicle);
	
	//Default values
	/*float connectionHeight = 1.2f; 
	float CUBE_HALF_EXTENTS = 1.0f; 
	float suspensionStiffness = 20.0f; 
	float suspensionDamping = 2.3f; 
	float suspensionCompression = 4.4f; 
	float rollInfluence = 0.1f;
	float wheelFriction = 1000; */
	float connectionHeight = 1.2f;
	//float connectionHeight = 0.0f; 
	//float CUBE_HALF_EXTENTS = 1.0f;
	float CUBE_HALF_EXTENTS = 1.75f;
	float suspensionStiffness = 200.0f;
	// k * 2.0 *btSqrt(m_suspensionStiffness)
	float suspensionDamping = .4f * 2.0f * sqrt(suspensionStiffness);
	float suspensionCompression = 3.0f * 2.0f * sqrt(suspensionStiffness); 
	float rollInfluence = 0.1f;
	float wheelFriction = 50;
	//vehicle->setCoordinateSystem(0, 1, 2);
	vehicle->setCoordinateSystem(0, 1, 2);


	// make wheels, also keep them under the half-length of chassis
	btVector3 wheelDirectionCS0(0.0f, -1.0f, 0.0f);
	btVector3 wheelAxleCS(-1.0f, 0.0f, 0.0f);
	//btScalar suspensionRestLength(0.6f);
	btScalar suspensionRestLength(0.6f);
	btScalar wheelRad(0.4f);
	btScalar suspensionForce(1.0f);

	//btVector3 connectionPointCS0(0, 0, -5);

	btVector3 connectionPointCS0(CUBE_HALF_EXTENTS - (0.3*wheelWidth), connectionHeight, 2 * CUBE_HALF_EXTENTS - wheelRadius);
	connectionPoints.push_back(connectionPointCS0); 
	vehicle->addWheel(connectionPointCS0, wheelDirectionCS0, wheelAxleCS, suspensionRestLength, wheelRadius, tune, true);

	connectionPointCS0 = btVector3(-CUBE_HALF_EXTENTS + (0.3*wheelWidth), connectionHeight, 2 * CUBE_HALF_EXTENTS - wheelRadius);
	connectionPoints.push_back(connectionPointCS0);
	vehicle->addWheel(connectionPointCS0, wheelDirectionCS0, wheelAxleCS, suspensionRestLength, wheelRadius, tune, true);

	connectionPointCS0 = btVector3(-CUBE_HALF_EXTENTS + (0.3*wheelWidth), connectionHeight, -2 * CUBE_HALF_EXTENTS + wheelRadius);
	connectionPoints.push_back(connectionPointCS0);
	vehicle->addWheel(connectionPointCS0, wheelDirectionCS0, wheelAxleCS, suspensionRestLength, wheelRadius, tune, false);

	connectionPointCS0 = btVector3(CUBE_HALF_EXTENTS - (0.3*wheelWidth), connectionHeight, -2 * CUBE_HALF_EXTENTS + wheelRadius);
	connectionPoints.push_back(connectionPointCS0);
	vehicle->addWheel(connectionPointCS0, wheelDirectionCS0, wheelAxleCS, suspensionRestLength, wheelRadius, tune, false);

	

	for (int i = 0; i < 3; i++)
	{
		btWheelInfo& wheel = vehicle->getWheelInfo(i);
		wheel.m_suspensionStiffness = suspensionStiffness;
		wheel.m_wheelsDampingRelaxation = suspensionDamping;
		//wheel.m_wheelsSuspensionForce = suspensionForce; 
		wheel.m_wheelsDampingCompression = suspensionCompression;
		wheel.m_frictionSlip = wheelFriction;
		wheel.m_rollInfluence = rollInfluence;
		wheel.m_maxSuspensionForce = 6000.0f;
	}
	
	//cout << vehicle->getChassisWorldTransform().getOrigin().getX() << " " << vehicle->getChassisWorldTransform().getOrigin().getY() << " " << vehicle->getChassisWorldTransform().getOrigin().getZ();

	// END VEHICLE -------------------------------------------------------------------------------------------------------------

	//Create Material 
	material = new Material(vertexShader, pixelShader, device, deviceContext, L"Textures/grey.png");
	carMaterial = new Material(vertexShader, pixelShader, device, deviceContext, L"Textures/red.png");
	ballMaterial = new Material(vertexShader, pixelShader, device, deviceContext, L"Textures/blue.png");


	
	//Create Debug Line Drawer and pass to dynamics world 
	drawDebug = new DrawDebug(device, deviceContext, cam, DrawDebugVertexShader, DrawDebugPixelShader);
	drawDebug->setDebugMode(btIDebugDraw::DBG_DrawWireframe);
	dynamicsWorld->setDebugDrawer(drawDebug); 

	
	
	//carMaterial = new Material(vertexShader, pixelShader, device, deviceContext, )
	//Create entities 
	e1 = new Entity(meshThree, material, shape1, groundMotionState, groundRigidBody);
	e2 = new Entity(meshTwo, ballMaterial, shape2, fallMotionState, fallRigidBody);
	e3 = new Entity(meshOne, carMaterial, chassisShape, chassisMotionState, chassisRB);

	/*e4 = new Entity(meshTwo, material, shape2, groundMotionState, fallRigidBody);
	e5 = new Entity(meshTwo, material, shape2, groundMotionState, fallRigidBody);
	e6 = new Entity(meshTwo, material, shape2, groundMotionState, fallRigidBody);
	e7 = new Entity(meshTwo, material, shape2, groundMotionState, fallRigidBody);*/




	//e2->move(XMFLOAT4(0, 5, 0, 0)); 
	//e2->move(XMFLOAT4(0, 10, 0, 0)); 
	//e1->move(XMFLOAT4(0, -7.5f, 2.0f ,0 ));
	e1->scale(XMFLOAT4(4.0f, 1.0f, 4.0f, 1.0f)); // ground
	//e2->move(XMFLOAT4(0, 10.0f, 13.5f, 0));
	//e2->scale(XMFLOAT4(2.0f, 2.0f, 2.0f, 1.0f)); 
	//organize entities in vector
	entities.push_back(e1);
	entities.push_back(e2); //Ball
	entities.push_back(e3); // car
	/*entities.push_back(e4);
	entities.push_back(e5);
	entities.push_back(e6);
	entities.push_back(e7);*/
	//entities.push_back(e3);
	//dynamicsWorld->addRigidBody(entities[0]->collider);
	//dynamicsWorld->addRigidBody(entities[1]->collider);

	//Create UI 
	//Selector = new HUD(device, deviceContext, L"Sprites/Sprite1.dds", 0.0f, 0.0f);
	Text = new HUD(device, deviceContext, L"SpriteFonts/Destroy_32.spritefont", L"Score: ", windowWidth / 2 - 400.0f, windowHeight / 2 - 325.0f);
	Time = new HUD(device, deviceContext, L"SpriteFonts/Destroy_32.spritefont", L"0", windowWidth / 2 - 200.0f, windowHeight / 2 - 325.0f);

	//UI.push_back(Selector);
	UI.push_back(Text);
	UI.push_back(Time); 
	score = 0; 
}


// --------------------------------------------------------
// Initializes the matrices necessary to represent our geometry's 
// transformations and our 3D camera
// --------------------------------------------------------
void MyDemoGame::CreateMatrices()
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
}

#pragma endregion

#pragma region Window Resizing

// --------------------------------------------------------
// Handles resizing DirectX "stuff" to match the (usually) new
// window size and updating our projection matrix to match
// --------------------------------------------------------
void MyDemoGame::OnResize()
{
	// Handle base-level DX resize stuff
	DirectXGameCore::OnResize();

	cam->onResize(aspectRatio);
}
#pragma endregion

#pragma region Game Loop

// --------------------------------------------------------
// Update your game here - take input, move objects, etc.
// --------------------------------------------------------
float x = 0;
void MyDemoGame::UpdateScene(float deltaTime, float totalTime)
{
	//Physics 
	UpdatePhysics(deltaTime); 

	// Quit if the escape key is pressed
	if (GetAsyncKeyState(VK_ESCAPE))
		Quit();

	if (GetAsyncKeyState(VK_UP))
	{
		engForce = maxEngineForce;
		brakeForce = 0.0f; 
	}
	if (GetAsyncKeyState(VK_LEFT))
	{
		steeringForce -= steeringIncrement;
		if (steeringForce < -steeringClamp)
			steeringForce = -steeringClamp;
		//cam->turn(-steeringForce * 10, 0.0f); 

	}
	if (GetAsyncKeyState(VK_RIGHT))
	{
		steeringForce += steeringIncrement;
		if (steeringForce > steeringClamp)
			steeringForce = steeringClamp;
		//cam->turn(steeringForce * 10, 0.0f);

		
	}
	if (GetAsyncKeyState('G'))
	{
		enableDebugDraw = !enableDebugDraw; 
	}
	if (GetAsyncKeyState(VK_DOWN))
	{
		brakeForce = maxBrakeForce; 
		engForce = 0.0f; 

	}

	for (int i = 0; i < 4; i++)
	{
		vehicle->updateWheelTransform(i, true);
		
	}



	btVector3 chaPos = vehicle->getChassisWorldTransform().getOrigin();

	if (chaPos.getY() < -30.0f)
	{
		score -= 1; 
		btTransform transform = e3->collider->getCenterOfMassTransform();
		transform.setOrigin(btVector3(0.0f, 0.0f, 0.0f));
		e3->collider->setCenterOfMassTransform(transform);
	}

	btVector3 ballPos = e2->collider->getWorldTransform().getOrigin(); 

	if (ballPos.getY() < -30.0f)
	{
		score += 1; 
		btTransform transform = e2->collider->getCenterOfMassTransform();
		transform.setOrigin(btVector3(0.0f, 20.0f, 0.0f));
		e2->collider->setLinearVelocity(btVector3(0.0f, 0.0f, 0.0f)); 
		e2->collider->setCenterOfMassTransform(transform);
	}

	/*btVector3 pos = e2->collider->getCenterOfMassTransform().getOrigin(); 
	e2->setPosition(pos.getX(), pos.getY(), pos.getZ()); */
	
	//e3->setPosition(chaPos.getX(), chaPos.getY(), chaPos.getZ());

	//btVector3 frRWheel = vehicle->getWheelInfo(0).m_worldTransform.getOrigin(); 
	////frRWheel = frRWheel - connectionPoints[0]; 

	//e4->setPosition(frRWheel.getX(), frRWheel.getY(), frRWheel.getZ());

	//btVector3 frLWheel = vehicle->getWheelInfo(1).m_worldTransform.getOrigin();
	////btVector3 frLWheel = connectionPoints[1];
	//e5->setPosition(frLWheel.getX(), frLWheel.getY(), frLWheel.getZ());


	//btVector3 baRWheel = vehicle->getWheelInfo(2).m_worldTransform.getOrigin();
	////btVector3 baRWheel = connectionPoints[2];
	//e6->setPosition(baRWheel.getX(), baRWheel.getY(), baRWheel.getZ());

	//btVector3 baLWheel = vehicle->getWheelInfo(3).m_worldTransform.getOrigin();
	////btVector3 baLWheel = connectionPoints[3];
	//e7->setPosition(baLWheel.getX(), baLWheel.getY(), baLWheel.getZ());

	

	//constants that control movement  
	float speed = 0.0025f * deltaTime;
	float rotation = 0.55f * deltaTime;
	float buffer = 1.5f;
	//update entities


	//Input --------------------------------------------

	//Check if mouse held
	if (leftmouseHeld) {
		vehicle->applyEngineForce(.20f, 0); 
		vehicle->applyEngineForce(.20f, 1);
		vehicle->applyEngineForce(.20f, 2);
		vehicle->applyEngineForce(.20f, 3);
	}
	//if (middlemouseHeld) { entities[1]->collider->applyCentralImpulse(btVector3(0.0f, 200.0f, 0.0f)); }
	pad->State.reset();
	 if (rightmouseHeld) 
	 { 
		 entities[1]->collider->applyImpulse(btVector3(0.0f, 50.0f, 0.0f), btVector3(0,0,0));  
	 }
	


	//Check if there is a game pad 
	 else if (pad->is_connected())
	 {
		 //Controller input is run a bunch of time because of FPS 
		 //Check state of Game Pad
		 pad->update();
		 //Quit with back button 
		 if (pad->State._buttons[GamePad_Button_BACK] == true)
		 {
			 Quit(); 
		 }
		 //Bounce Ball with A 
		 else if (pad->State._buttons[GamePad_Button_A] == true)
		 {
			 //Make value small because of FPS 
			 cout << "a pressed";
			 //entities[1]->collider->applyImpulse(btVector3(0.0f, 0.1f, 0.0f), btVector3(0, 0, 0));
			 
			 vehicle->applyEngineForce(.20f, 0);
			 vehicle->applyEngineForce(.20f, 1);
			 vehicle->applyEngineForce(.20f, 2);
			 vehicle->applyEngineForce(.20f, 3);
		 }
		 
		 
	 }

	 


	//Update all entities in the world
	for (unsigned int i = 0; i < entities.size(); i++)
	{
		entities[i]->updateScene();
	}

	//Update UI 

	if (score == 0)
	{
		UI[1]->changeText(L"0");
	}
	else if (score == 1)
	{
		UI[1]->changeText(L"1");
	}
	else if (score == 2)
	{
		UI[1]->changeText(L"2");
	}
	else if (score >= 3)
	{
		UI[1]->changeText(L"You Won!");
	}
	
	
	
	
	 
	for (unsigned int i = 0; i < UI.size(); i++)
	{
		UI[i]->Update();
	}


	//update Camera and it's input
	cam->cameraInput(deltaTime);
	cam->follow(XMFLOAT3(chaPos.getX(), chaPos.getY(), chaPos.getZ())); 
	cam->update(deltaTime);
	drawDebug->Update(); 
}



// --------------------------------------------------------
// Clear the screen, redraw everything, present to the user
// --------------------------------------------------------
void MyDemoGame::DrawScene(float deltaTime, float totalTime)
{
	// Background color (Cornflower Blue in this case) for clearing
	const float color[4] = { 0.4f, 0.6f, 0.75f, 0.0f };
	//const float color[4] = { 0.0f, 0.0f, 0.0f, 0.0f };


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
	
	for (unsigned int i = 0; i < entities.size(); i++)
	{
		// Send data to shader variables
		//  - Do this ONCE PER OBJECT you're drawing
		//  - This is actually a complex process of copying data to a local buffer
		//    and then copying that entire buffer to the GPU.  
		//  - The "SimpleShader" class handles all of that for you.
		entities[i]->prepareMaterial(cam->getViewMatrix(), cam->getProjectionMatrix());
		//draw here 
		if (i != 2 && enableDebugDraw)
		{
			entities[i]->drawScene(deviceContext);
		}
		else if (enableDebugDraw == false)
		{
			entities[i]->drawScene(deviceContext);
		}
	}
	//Testing debug lines 
	//Debug lines 
	if (enableDebugDraw)
	{
		dynamicsWorld->debugDrawWorld();
	}

	//Draw UI
	for (unsigned int i = 0; i < UI.size(); i++)
	{
		UI[i]->Render();
	}
	
	//drawDebug->drawLine(btVector3(0.0f, 0.0f, 0.0f), btVector3(0.0f, 1.0f, 1.0f), btVector3(1.0f, 0.0f, 0.0f)); 
	DrawDebugVertexShader->SetShader(true);
	DrawDebugPixelShader->SetShader(true);
	if (enableDebugDraw)
	{
		drawDebug->Draw();
	}
	
	

	//deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


	
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
void MyDemoGame::OnMouseDown(WPARAM btnState, int x, int y)
{
	// Save the previous mouse position, so we have it for the future
	prevMousePos.x = x;
	prevMousePos.y = y;
	cout << "Mouse down" << endl; 
	//mouse input
	if (btnState & 0x0001) { /* Left button is down */ leftmouseHeld = true; }

	if (btnState & 0x0002) { /* Right button is down */ rightmouseHeld = true; }

	if (btnState & 0x0010) { /* Middle button is down */ middlemouseHeld = true; }




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
void MyDemoGame::OnMouseUp(WPARAM btnState, int x, int y)
{
	// We don't care about the tracking the cursor outside
	// the window anymore (we're not dragging if the mouse is up)
	cout << "Mouse up" << endl;

	if (btnState & 0x0001) { /* Left button is down */ leftmouseHeld = false; }

	if (btnState & 0x0002) { /* Right button is down */ rightmouseHeld = false; }

	if (btnState & 0x0010) { /* Middle button is down */ middlemouseHeld = false; }


	ReleaseCapture();
}

// --------------------------------------------------------
// Helper method for mouse movement.  We only get this message
// if the mouse is currently over the window, or if we're 
// currently capturing the mouse.
//
// Feel free to add code to this method
// --------------------------------------------------------
void MyDemoGame::OnMouseMove(WPARAM btnState, int x, int y)
{
	//calc Cam coords
	float camX = x - prevMousePos.x;
	float camY = y - prevMousePos.y;
	// Save the previous mouse position, so we have it for the future
	prevMousePos.x = x;
	prevMousePos.y = y;


	//cam->turn(camX, camY);
}
#pragma endregion