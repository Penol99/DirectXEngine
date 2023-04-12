#pragma once
#include "windows.h"
#include "Mouse/MouseInput.h"
#include "Keyboard/KeyboardInput.h"
class InputHandler
{
private:
	KeyboardInput mKeyboard;
	MouseInput mMouse;
public:
	bool UpdateEvents(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam); // True if raw input
	inline KeyboardInput Keyboard() { return mKeyboard; }
	inline MouseInput Mouse() { return mMouse; }
};