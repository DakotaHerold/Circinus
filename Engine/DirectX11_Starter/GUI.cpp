#include "GUI.h"



GUI::GUI()
{
	
}

void GUI::Init(HWND hwnd, ID3D11Device* device, ID3D11DeviceContext* device_context)
{

	if (!ImGui_ImplDX11_Init(hwnd, device, device_context)) {
		MessageBox(hwnd, L"ImGUI Init Failed", L"Warning", 0);
	}

}

void GUI::Draw()
{

	ImGui::Render();

}

void GUI::Update()
{

	// Use this to Update the frames. I.e, create new stuff I guess.
	ImGui_ImplDX11_NewFrame();
	{
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}

	AddMenuBar();

	// Debug Properties
	ImGui::Begin("Mouse and Keyboard Status");
	{
		ImGui::Text("GUI %s Mouse Input ", (ImGui::GetIO().WantCaptureMouse) ? "wants" : "doesn't want");
		ImGui::Text("GUI %s Keyboard Input ", (ImGui::GetIO().WantCaptureKeyboard ) ? "wants" : "doesn't want");
		ImGui::Text("Mouse %s Down", ImGui::IsMouseDown(0) ? "" : "not");

		ImGui::End();
	}

}

void GUI::AddMenuBar() {

	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			ShowExampleMenuFile();
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Edit"))
		{
			if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
			if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
			ImGui::Separator();
			if (ImGui::MenuItem("Cut", "CTRL+X")) {}
			if (ImGui::MenuItem("Copy", "CTRL+C")) {}
			if (ImGui::MenuItem("Paste", "CTRL+V")) {}
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
