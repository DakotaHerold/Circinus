#pragma once

#pragma region Import ImGUI
//#include "ImGui\imgui_impl_dx11.cpp"
#include "ImGui\imgui_impl_dx11.h"
#pragma endregion

#include <Windows.h>
#include "NativeWindow.h"

class GUI
{

private:

	bool running = true;

	static GUI Instance;

	GUI();

public:

	// Singleton reference
	static GUI& instance()
	{
		static GUI gui_instance;
		return gui_instance;
	}

	void Init(HWND hwnd, ID3D11Device* device, ID3D11DeviceContext* device_context);
	void Draw();
	bool Update(int _windowWidth, int _windowHeight);
	void End();
	void AddMenuBar();
	void ShowExampleMenuFile();

	~GUI(); // Fix the Destructor. Search for Singletons in C++


};

