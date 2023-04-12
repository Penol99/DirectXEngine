#pragma once
#include "RenderWindow.h"
#include "Input/InputHandler.h"
#include "Graphics/Graphics.h"
class WindowContainer
{
public:
	WindowContainer();
	LRESULT WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
protected:
	RenderWindow mRenderWindow;
	Graphics mGfx;
	InputHandler mInput;
private:

};