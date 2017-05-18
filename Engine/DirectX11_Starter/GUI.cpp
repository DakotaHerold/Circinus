#include "GUI.h"

#ifdef HAS_GUI

#include "Engine.h"
#include "Editor.h"
#include "Scene.h"
#include "Entity.h"
#include <math.h>
#include "RenderingSystem.h"
#include "ScriptComponent.h"
#include "TransformSystem.h"
#include "Editor.h"
#include "SceneManager.h"
#include "RigidBody.h"
#include "Renderable.h"
#include <string>
#include <sys/stat.h>
#include <unordered_map>

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
	// _cwFlag |= ImGuiWindowFlags_NoMove;
	_cwFlag |= ImGuiWindowFlags_NoSavedSettings;
	// _cwFlag |= ImGuiWindowFlags_NoResize;

	// Component Details Window Flags.
	_cdwFlag |= ImGuiWindowFlags_NoSavedSettings;

	// Benchmarks Window Flags.
	_bwFlag |= ImGuiWindowFlags_NoMove;
	_bwFlag |= ImGuiWindowFlags_NoSavedSettings;


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

#ifdef HAS_EDITOR
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

			//vector<Entity *> curSceneEntities = Engine::instance()->GetCurScene()->GetAllEntities();
			//int entCounter = 0;

			if (nullptr != Editor::instance()->GetSelectedEntity()) {
				selectedEntity = Editor::instance()->GetSelectedEntity();
				ComponentDisplayFlag = true;
				_somthingSelected = true;
			}

			//// TODO: Make sure to parent and child the objects accordingly.
			//for (std::vector<Entity *>::iterator it = curSceneEntities.begin(); it != curSceneEntities.end(); ++it) {
			//	++entCounter;
			//	if (ImGui::Selectable(((*it)->GetName() + " (" + std::to_string(entCounter) + ")").c_str())) {
			//		selectedEntity = *it;
			//		Editor::instance()->SetSelectedEntity(*it);
			//		ComponentDisplayFlag = true;
			//		_somthingSelected = true;
			//	}
			//}

			Transform* root = Engine::instance()->GetCurScene()->root;
			for (auto i = root->children.begin(); i != root->children.end(); ++i)
			{
				HierarchyTree(*i);
			}

			ImGui::End();
		}
	}
#endif

	// Component Window
	if (nullptr != selectedEntity && ComponentDisplayFlag) {

		// TODO: Can probably move these to Init Function.
		ImVec2 cwPos = ImVec2(2 * _windowWidth / 3, _windowHeight / 2 + 20);
		ImVec2 cwSize = ImVec2(_windowWidth / 3, _windowHeight / 2 - 20);
		ImGui::SetNextWindowPos(cwPos, ImGuiSetCond_FirstUseEver);
		ImGui::SetNextWindowSize(cwSize, ImGuiSetCond_FirstUseEver);

		ImGui::Begin(("Entity: " + selectedEntity->GetName()).c_str(), &ComponentDisplayFlag, _cwFlag);
		{
			char bufEntityName[512] = {};
			strcpy(bufEntityName, selectedEntity->GetName().c_str());
			if (ImGui::InputText("Name", bufEntityName, 512, ImGuiInputTextFlags_EnterReturnsTrue))
			{
				selectedEntity->ChangeName(bufEntityName);
			}

			int compCounter = 0;
			vector<pair<TypeId, ObjectPoolIndex *>> Components;
			Components = cm->GetAllComponentsInfo((selectedEntity)->GetID());
			for (vector<pair<TypeId, ObjectPoolIndex *>>::iterator that = Components.begin(); that != Components.end(); ++that) {
				++compCounter;
				//std::string tempString = (ComponentTypeName(that->first)) + std::to_string(entCounter) + "-" + std::to_string(compCounter);
				if (ImGui::Selectable(((ComponentTypeName(that->first)) + std::string(" (") + /* std::to_string(entCounter) + */ "-" + std::to_string(compCounter) + std::string(")")).c_str())) {
					// We need to select the Component here.
					selectedComponentID = that->first;
					selectedCompIndex = that->second;
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
							_cwFlag = false;
							cm->RemoveComponent((selectedEntity)->GetID(), that->first);
						}
						ImGui::EndPopup();
					}
				}
			}

			ImGui::Separator();

			do // using a do-while(0), just so we can use break to jump to the end of the block
			{
				char* typeName = ComponentTypeName(selectedComponentID);

				// Component Details Window
				if (0 == std::strcmp(typeName, "class ScriptComponent"))
				{
					auto comp = cm->GetComponent<ScriptComponent>((selectedEntity)->GetID(), selectedCompIndex);
					if (nullptr == comp)
						break;

					char buf[1024] = {};
					strcpy(buf, comp->GetScriptName().c_str());

					if (ImGui::InputText(".lua", buf, 1024, ImGuiInputTextFlags_EnterReturnsTrue))
					{
						std::string newFileName(buf);
						if (newFileName != comp->GetScriptName())
						{
							newFileName = "Scripts/" + newFileName + ".lua";

							// test if file exists
							struct stat buffer;
							if (stat(newFileName.c_str(), &buffer) != 0)
								break;

							comp->ResetScript(buf);
						}
					}

					auto& table = comp->GetParameterTable();
					ImGui::Text("Parameters");
					if (ImGui::Button("Add"))
					{
						char bufk[512] = {};
						size_t i = table.size();
						do
						{
							sprintf(bufk, "NewKey%lu", i++);
						} while (table.find(bufk) != table.end());
						
						table.insert(std::pair<std::string, std::string>(bufk, ""));
					}

					static std::string keyToDel;
					static std::string keyToAdd;
					static std::string valToAdd;

					keyToDel.clear();
					keyToAdd.clear();
					valToAdd.clear();

					{
						char buf[1024];
						char bufK[512], bufV[512];

						for (auto i = table.begin(); i != table.end(); ++i)
						{
							sprintf(buf, "%s: %s", i->first.c_str(), i->second.c_str());
							ImGui::Text(buf);
							if (ImGui::Button(("Modify " + i->first).c_str()))
							{
								selectedKeyInParameterTable = i->first;
							}

							if (selectedKeyInParameterTable == i->first)
							{
								strcpy(bufK, i->first.c_str());
								strcpy(bufV, i->second.c_str());
								bool k = ImGui::InputText("Key", bufK, 512, ImGuiInputTextFlags_EnterReturnsTrue);
								bool v = ImGui::InputText("Value", bufV, 512, ImGuiInputTextFlags_EnterReturnsTrue);
								if (k || v)
								{
									selectedKeyInParameterTable.clear();
									std::string newKey(bufK);
									std::string newValue(bufV);
									if (newKey == i->first)
									{
										table[newKey] = newValue;
									}
									else
									{
										keyToDel = i->first;
										keyToAdd = newKey;
										valToAdd = newValue;
									}
								}
							}

							if (ImGui::Button(("Delete " + i->first).c_str()))
							{
								keyToDel = i->first;
							}
						}
					}

					if (!keyToDel.empty())
					{
						table.erase(keyToDel);
					}
					if (!keyToAdd.empty())
					{
						table.insert(std::pair<std::string, std::string>(keyToAdd, valToAdd));
					}
				}
				else if (0 == std::strcmp(typeName, "class Transform")) 
				{
					auto comp = cm->GetComponent<Transform>((selectedEntity)->GetID(), selectedCompIndex);
					if (nullptr == comp)
						break;

					guiPos = *(comp->GetWorldPosition());
					ImGui::Text("Position: ");
					guiUpPos = guiPos;

					ImGui::InputFloat("x", &(guiUpPos.x), 0.1f, 1.0f);
					ImGui::InputFloat("y", &(guiUpPos.y), 0.1f, 1.0f);
					ImGui::InputFloat("z", &(guiUpPos.z), 0.1f, 1.0f);

					if (guiPos.x != guiUpPos.x || guiPos.y != guiUpPos.y || guiPos.z != guiUpPos.z) {
						comp->SetWorldPosition((guiUpPos).x, (guiUpPos).y, (guiUpPos).z);
					}
				}
				else if (0 == std::strcmp(typeName, "class Renderable"))
				{
					auto comp = cm->GetComponent<Renderable>((selectedEntity)->GetID(), selectedCompIndex);
					if (nullptr == comp)
						break;

					ImGui::Text("Mesh: ");// comp->mesh->filename.c_str());
					char buf[1024];
					strcpy(buf, comp->mesh->filename.c_str());
					
					if (ImGui::InputText("Mesh", buf, 1024, ImGuiInputTextFlags_EnterReturnsTrue))
					{
						std::string newFileName(buf);

						if (newFileName == comp->mesh->filename)
							break;

						std::wstring newFileName_w(newFileName.begin(), newFileName.end());
						Mesh* m = RenderingSystem::instance()->CreateMesh(newFileName_w.c_str());
						if (nullptr != m)
						{
							comp->mesh = m;
						}
					}
				}
				else 
				{
					ImGui::Text(typeName);
				}
			} while (0);

			ImGui::End();
		}
	}

	if (BenchmarkDisplayFlag) {
		// TODO: Can probably move these to Init Function.
		ImVec2 hwPos = ImVec2(30, 50);
		ImVec2 hwSize = ImVec2(_windowWidth / 3, _windowHeight / 3);
		ImGui::SetNextWindowPos(hwPos, ImGuiSetCond_FirstUseEver);
		ImGui::SetNextWindowSize(hwSize, ImGuiSetCond_FirstUseEver);
		ImGui::Begin("Benchmarks.", &BenchmarkDisplayFlag, _bwFlag);
		{
			ImGui::Text("== Benchmarks ==");
			ImGui::Separator();
			ImGui::Text("Current Number of Entities: %d", Engine::instance()->GetCurScene()->GetAllEntities().size());
			ImGui::Separator();
			ImGui::InputInt("No. of Entities to Create", &numberOfEntitiesToCreate, 50, 200, 0);
			if (ImGui::Button("Create Entities") && numberOfEntitiesToCreate > 0) {
				int count = numberOfEntitiesToCreate;
				float range = 4.0;
				float start = -range / 2;

				int row_column = sqrt(count);
				float offset = row_column == 1 ? 0 : range / (row_column - 1);

				RenderingSystem& renderer = *RenderingSystem::instance();

				Mesh* mesh = renderer.CreateMesh(L"Assets/Models/cube.fbx");
				Shader* shader = renderer.CreateShader(L"Assets/ShaderObjs/Opaque.cso");
				Texture* tex = renderer.CreateTexture(L"Assets/Textures/rust.jpg");
				Material *mat = renderer.CreateMaterial(shader);
				mat->SetTexture("texDiffuse", tex);

				for (int i = 0; i < row_column; i++) {
					for (int j = 0; j < row_column; j++) {
						Entity* e = new Entity();
						Transform* t = e->GetComponent<Transform>();
						t->SetLocalPosition(start + offset * j, start + offset * i, 0);

						Renderable* r = e->AddComponent<Renderable>(mesh, mat);

						Engine::instance()->GetCurScene()->AddEntity(e);
					}
					/*
								RigidBody* rb1 = e1->AddComponent <RigidBody>(t1, &(r1->BoundingBox()));*/
								//e1->AddComponent<ScriptComponent>("script2.lua", rb1);
				}
			}
			ImGui::Separator();
			ImGui::InputInt("No. of Entities to Destroy", &numberOfEntitiesToCreate, 50, 200, 0);
			if (ImGui::Button("Destroy Entities")) {
				int count = numberOfEntitiesToCreate + 1; // + 1 to offset some bug in the code. ( < instead of <= I guess? )
				// Insert Logic to destroy the Entities here.
			}
			ImGui::End();
		}
	}

}

void GUI::AddMenuBar(bool * _running) {

	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("Engine")) {
			if (ImGui::MenuItem("Load")) {
				// TODO: Check if we can load a scene file, by taking in the Input.
				Engine::instance()->LoadScene("Scene1");
			}
			if (ImGui::MenuItem("Save")) {
				Engine::instance()->SavaScene();
			}
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
			if (ImGui::MenuItem("Benchmarks")) { BenchmarkDisplayFlag = true; }

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Entity"))
		{
			if (ImGui::BeginMenu("Add"))
			{
				if (ImGui::MenuItem("Empty")) {
					Engine::instance()->GetCurScene()->CreateEntity("Empty");
				}
				ImGui::EndMenu();
			}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Component")) {
			if (ImGui::BeginMenu("Add")) {
				if (ImGui::BeginMenu("Renderer")) {
					// TODO: Add a renderer component?
					if (ImGui::MenuItem("Cube")) {
						RenderingSystem& renderer = *RenderingSystem::instance();

						Mesh* mesh = renderer.CreateMesh(L"Assets/Models/cube.fbx");
						Shader* shader = renderer.CreateShader(L"Assets/ShaderObjs/Opaque.cso");
						Texture* tex = renderer.CreateTexture(L"Assets/Textures/rust.jpg");
						Material *mat = renderer.CreateMaterial(shader);
						mat->SetTexture("texDiffuse", tex);

						selectedEntity->AddComponent<Renderable>(mesh, mat);

					}
					ImGui::EndMenu();
				}

				if (ImGui::BeginMenu("RigidBody")) {
					// TODO: Add a rigidbody component
					if (ImGui::MenuItem("BoxCollider")) {
						XMFLOAT3 scale = *(selectedEntity->GetComponent<Transform>()->GetWorldScale());
						scale.x = (scale.x > 0) ? scale.x : 1;
						scale.y = (scale.y > 0) ? scale.y : 1;
						scale.z = (scale.z > 0) ? scale.z : 1;
						BoundingBox tempBound = BoundingBox(*(selectedEntity->GetComponent<Transform>()->GetWorldPosition()), scale);
						selectedEntity->AddComponent<RigidBody>(selectedEntity->GetComponent<Transform>(), &tempBound);
					}

					ImGui::EndMenu();
				}

				if (ImGui::MenuItem("ScriptComponent")) {
					// TODO: Add a script copmonent
				}
				ImGui::EndMenu();
			}
			ImGui::EndMenu();
		}

#ifdef HAS_EDITOR
		if (ImGui::BeginMenu("Run"))
		{
			Editor* editor = Editor::instance();

			if (ImGui::MenuItem(editor->IsPlaying() ? "[Play]" : "Play")) {
				editor->Play();
			}

			if (ImGui::MenuItem(editor->IsPaused() ? "[Pause]" : "Pause")) {
				editor->Pause();
			}

			if (ImGui::MenuItem("Stop")) {
				editor->Stop();
			}

			if (ImGui::MenuItem("Run Standalone")) {
				Editor::instance()->RunStandalone();
			}

			if (ImGui::MenuItem("Build")) {
				Editor::instance()->Build();
			}
			ImGui::EndMenu();
		}
#endif
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

void GUI::HierarchyTree(Transform * t)
{
	static ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;
	Entity* e = t->GetEntity();
	string nodeName = (e->GetName() + " (" + std::to_string(t->GetEntityID()) + ")");
	bool isOpen = ImGui::TreeNodeEx(nodeName.c_str(), node_flags);
	if (ImGui::IsItemClicked())
	{
		selectedEntity = e;
		Editor::instance()->SetSelectedEntity(e);
		ComponentDisplayFlag = true;
	}
	
	{
		if (ImGui::BeginPopupContextItem((nodeName + " Popup").c_str())) {
			if (ImGui::Button("Add Child")) {
				Entity * tempEnt = Engine::instance()->GetCurScene()->CreateEntity("Empty");
				tempEnt->transform->SetParent(t);
				ImGui::CloseCurrentPopup();
			}
			if (ImGui::Button("Delete")) {
				// TODO !!

				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
	}

	if (isOpen)
	{
		for (auto i = t->children.begin(); i != t->children.end(); ++i)
		{
			HierarchyTree(*i);
		}
		ImGui::TreePop();
	}
}

void GUI::End()
{
	ImGui_ImplDX11_InvalidateDeviceObjects();
	ImGui::Shutdown();
}


GUI::~GUI()
{

}

#endif
