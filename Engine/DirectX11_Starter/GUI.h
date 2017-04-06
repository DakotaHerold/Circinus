#pragma once

#pragma region Import ImGUI
#include "ImGui\imgui_impl_dx11.h"
#pragma endregion

#include <Windows.h>

class GUI
{

	// Singleton reference
	static GUI& instance()
	{
		static GUI gui_instance;
		return gui_instance;
	}

private: 

	GUI();

public:
	
	void Init(HWND hwnd, ID3D11Device* device, ID3D11DeviceContext* device_context);

	~GUI(); // Fix the Destructor. Search for Singletons in C++


};

