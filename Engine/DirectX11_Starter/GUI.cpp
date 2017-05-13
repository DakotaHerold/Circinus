#include "GUI.h"
#include "Engine.h"
#include "Scene.h"
#include "Entity.h"
#include <math.h>
#include "RenderingSystem.h"

GUI::GUI()
{
	
}

void GUI::Init(HWND hwnd, ID3D11Device* device, ID3D11DeviceContext* device_context)
{

	if (!ImGui_ImplDX11_Init(hwnd, device, device_context)) {
		MessageBox(hwnd, L"ImGUI Init Failed", L"Warning", 0);
	}

	// Window Flag Constants

	// Hierarchy Window Flags.
	_hwFlag |= ImGuiWindowFlags_NoMove;
	_hwFlag |= ImGuiWindowFlags_NoSavedSettings;
	_hwFlag |= ImGuiWindowFlags_NoResize;

	// Component Window Flags.
	_cwFlag |= ImGuiWindowFlags_NoMove;
	_cwFlag |= ImGuiWindowFlags_NoSavedSettings;
	_cwFlag |= ImGuiWindowFlags_NoResize;

	// Component Details Window Flags.
	_cdwFlag |= ImGuiWindowFlags_NoSavedSettings;

}

void GUI::Draw()
{

	ImGui::Render();

}

void GUI::Update(int _windowWidth, int _windowHeight, bool * _running)
{
	cm = ComponentManager::current;

	// Use this to Update the frames. I.e, create new stuff I guess.
	ImGui_ImplDX11_NewFrame();

	AddMenuBar(_running);

	// Debug Properties
	if (DebugDisplayFlag) {
		ImGui::Begin("Mouse and Keyboard Status", &DebugDisplayFlag);
		{
			ImGui::Text("GUI %s Mouse Input ", (ImGui::GetIO().WantCaptureMouse) ? "wants" : "doesn't want");
			ImGui::Text("GUI %s Keyboard Input ", (ImGui::GetIO().WantCaptureKeyboard) ? "wants" : "doesn't want");
			ImGui::Text("Mouse %s Down", ImGui::IsMouseDown(0) ? "" : "not");
			ImGui::Text("Mouse %s Clicked", ImGui::IsMouseClicked ? "" : "not");

			ImGui::End();
		}
	}

	// Temp Variable to hold the selection thing.
	bool _somthingSelected = false;
	bool _someComponentSelected = false;

	// Hierarchy Window
	if (HierarchyDisplayFlag) {

		// TODO: Can probably move these to Init Function.
		ImVec2 hwPos = ImVec2(2 * _windowWidth / 3, 20);
		ImVec2 hwSize = ImVec2(_windowWidth / 3, _windowHeight / 2);
		ImGui::SetNextWindowPos(hwPos);
		ImGui::SetNextWindowSize(hwSize);
		ImGui::Begin("Hierarchy", &HierarchyDisplayFlag, _hwFlag);
		{
			ImGui::Text("Hierarchy");
			ImGui::Separator();

			vector<Entity *> curSceneEntities = Engine::instance()->GetCurScene()->GetAllEntities();
			int entCounter = 0;

			for (std::vector<Entity *>::iterator it = curSceneEntities.begin(); it != curSceneEntities.end(); ++it) {
				++entCounter;
				if (ImGui::Selectable(((*it)->GetName() + " (" + std::to_string(entCounter) + ")").c_str())) {
					selectedEntity = *it;
					ComponentDisplayFlag = true;
					_somthingSelected = true;
				}
			}

			ImGui::End();
		}
	}

	// Component Window
	if (nullptr != selectedEntity && ComponentDisplayFlag) {

		// TODO: Can probably move these to Init Function.
		ImVec2 cwPos = ImVec2(2 * _windowWidth / 3,  _windowHeight/2 + 20);
		ImVec2 cwSize = ImVec2(_windowWidth / 3, _windowHeight / 2 - 20);
		ImGui::SetNextWindowPos(cwPos);
		ImGui::SetNextWindowSize(cwSize);

		ImGui::Begin("Component", &ComponentDisplayFlag, _cwFlag);
		{
			int compCounter = 0;
			vector<pair<TypeId, ObjectPoolIndex *>> Components;
			Components = cm->GetAllComponents((selectedEntity)->GetID());
			for (vector<pair<TypeId, ObjectPoolIndex *>>::iterator that = Components.begin(); that != Components.end(); ++that) {
				++compCounter;
				//std::string tempString = (ComponentTypeName(that->first)) + std::to_string(entCounter) + "-" + std::to_string(compCounter);
				if (ImGui::Selectable(((ComponentTypeName(that->first)) + std::string(" (") + /* std::to_string(entCounter) + */ "-" + std::to_string(compCounter) + std::string(")")).c_str())) {
					// We need to select the Component here.
					selectedComponentID = that->first;
					selectedCompIndex = that->second;
					ComponentDisplayDetailsFlag = true;
				}
				{
					if (ImGui::BeginPopupContextItem(((ComponentTypeName(that->first)) + std::string(" (") /* + std::to_string(entCounter) */ + "-" + std::to_string(compCounter) + std::string(") Popup")).c_str())) {
						if (ImGui::Button("Rename")) {
							// Popup something here.
						}
						ImGui::Button("Duplicate");
						ImGui::Separator();
						if (ImGui::Button("Delete")) {
							// Popup yes/no?
							cm->RemoveComponent((selectedEntity)->GetID(), that->first);
						}
						ImGui::EndPopup();
					}
				}
			}
			ImGui::End();
		}
	}

	// Component Details Window
	if (NULL != selectedComponentID && ComponentDisplayDetailsFlag) {
		// TODO: Can probably move these to Init Function.
		// ImVec2 cwPos = ImVec2(30, _windowHeight / 2 + 20);
		ImVec2 cwSize = ImVec2(_windowWidth / 3, _windowHeight / 2 - 20);
		// ImGui::SetNextWindowPos(cwPos);
		ImGui::SetNextWindowSize(cwSize);
		ImGui::Begin("Component Details", &ComponentDisplayDetailsFlag, _cdwFlag);
		{
			if ( 0 == std::strcmp(ComponentTypeName(selectedComponentID), "class ScriptComponent") )
			{
				//ImGui::Text( GetComponent(selectedEntity, selectedCompIndex))
				// TODO:: ERROR -- FIX IT
				// cm->GetComponent((selectedEntity)->GetID(), selectedComponentID, selectedCompIndex);
			}
			else {
				std::cout << "?" << ComponentTypeName(selectedComponentID) << "?" << std::endl;
				std::cout << "#" << std::strcmp(ComponentTypeName(selectedComponentID), "class ScriptComponent") << "#" << std::endl;
			}
			ImGui::End();
		}
	}

}

void GUI::AddMenuBar(bool * _running) {

	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("Engine")) {
			ImGui::MenuItem("Preferences");
			ImGui::Separator();
			if (ImGui::MenuItem("Quit", "ALT+F4")) { *(_running) = false; }
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Window"))
		{
			if (ImGui::MenuItem("Hierarchy")) { HierarchyDisplayFlag = true; }
			ImGui::Separator();
			if (ImGui::MenuItem("Debug")) {
				DebugDisplayFlag = true;
			}
			
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Entity"))
		{
			if (ImGui::BeginMenu("Add"))
			{
				// TODO: Add functionality.
				// What should I add?
				ImGui::EndMenu();
			}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Benchmarks")) {
			if (ImGui::MenuItem("Add 1000 Objects")) {
				int count = 100;
				float range = 4.0;
				float start = -range / 2;

				int row_column = sqrt(count);
				float offset = range / (row_column - 1);

				RenderingSystem& renderer = *RenderingSystem::instance();

				Mesh* mesh = renderer.CreateMesh("Assets/Models/cube.fbx");
				Shader* shader = renderer.CreateShader(L"Assets/ShaderObjs/Opaque.cso");
				Texture* tex = renderer.CreateTexture(L"Assets/Textures/rust.jpg");
				Material *mat = renderer.CreateMaterial(shader);
				mat->SetTexture("texDiffuse", tex);

				for (int i = 0; i < row_column; i++) {
					for (int j = 0; j < row_column; j++) {
						Entity* e = new Entity();
						Transform* t = e->AddComponent<Transform>();
						t->SetLocalPosition(start + offset * j, start + offset * i, 0);

						//Renderable* r = e->AddComponent<Renderable>(mesh, mat);

						Engine::instance()->GetCurScene()->AddEntity(e);
					}	
		/*			
					RigidBody* rb1 = e1->AddComponent <RigidBody>(t1, &(r1->BoundingBox()));*/
					//e1->AddComponent<ScriptComponent>("script2.lua", rb1);
				}

			}
			if (ImGui::MenuItem("Add 2000 Objects")) {
				
			}
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}

}

void GUI::ShowExampleMenuFile()
{
	ImGui::MenuItem("(dummy menu)", NULL, false, false);
	if (ImGui::MenuItem("New")) {}
	if (ImGui::MenuItem("Open", "Ctrl+O")) {}
	if (ImGui::BeginMenu("Open Recent"))
	{
		ImGui::MenuItem("fish_hat.c");
		ImGui::MenuItem("fish_hat.inl");
		ImGui::MenuItem("fish_hat.h");
		if (ImGui::BeginMenu("More.."))
		{
			ImGui::MenuItem("Hello");
			ImGui::MenuItem("Sailor");
			if (ImGui::BeginMenu("Recurse.."))
			{
				ShowExampleMenuFile();
				ImGui::EndMenu();
			}
			ImGui::EndMenu();
		}
		ImGui::EndMenu();
	}
	if (ImGui::MenuItem("Save", "Ctrl+S")) {}
	if (ImGui::MenuItem("Save As..")) {}
	ImGui::Separator();
	if (ImGui::BeginMenu("Options"))
	{
		static bool enabled = true;
		ImGui::MenuItem("Enabled", "", &enabled);
		ImGui::BeginChild("child", ImVec2(0, 60), true);
		for (int i = 0; i < 10; i++)
			ImGui::Text("Scrolling Text %d", i);
		ImGui::EndChild();
		static float f = 0.5f;
		static int n = 0;
		ImGui::SliderFloat("Value", &f, 0.0f, 1.0f);
		ImGui::InputFloat("Input", &f, 0.1f);
		ImGui::Combo("Combo", &n, "Yes\0No\0Maybe\0\0");
		ImGui::EndMenu();
	}
	if (ImGui::BeginMenu("Colors"))
	{
		for (int i = 0; i < ImGuiCol_COUNT; i++)
			ImGui::MenuItem(ImGui::GetStyleColName((ImGuiCol)i));
		ImGui::EndMenu();
	}
	if (ImGui::BeginMenu("Disabled", false)) // Disabled
	{
		IM_ASSERT(0);
	}
	if (ImGui::MenuItem("Checked", NULL, true)) {}
	if (ImGui::MenuItem("Quit", "Alt+F4")) {}
}

void GUI::End()
{
	ImGui_ImplDX11_InvalidateDeviceObjects();
	ImGui::Shutdown();
}


GUI::~GUI()
{

}
