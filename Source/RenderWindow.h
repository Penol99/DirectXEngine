#pragma once
#include "ErrorLog.h"

class WindowContainer;

class RenderWindow
{
public:
	bool Init(WindowContainer* pWindowContainer, HINSTANCE hInstance, std::string aWindowTitle, std::string aWindowClass, int aWidth, int aHeight);
	bool ProcessMessages();
	HWND GetHWND() const;
	~RenderWindow();
private:
	void RegisterWindowClass();
	HWND mHandle = NULL; // Handle to this window;
	HINSTANCE hInstance = NULL; // Handle to this application instance
	std::string mWindowTitle = "";
	std::wstring mWindowTitleWide = L"";
	std::string mWindowClass = "";
	std::wstring mWindowClassWide =  L"";
	int mWidth = 0;
	int mHeight = 0;

};