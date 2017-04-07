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
		ImGui::Text("Hello");
	}

}

GUI::~GUI()
{

}
