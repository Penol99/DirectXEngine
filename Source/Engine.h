#pragma once
#include "WindowContainer.h"
class Engine : WindowContainer
{
public:
	bool Initialize(HINSTANCE hInstance, std::string aWindowTitle, std::string aWindowClass, int aWidth, int aHeight);
	bool ProcessMessages();

	void Update();
	void RenderFrame();
};