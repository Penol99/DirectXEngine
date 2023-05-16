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
	HWND myHandle = NULL; // Handle to this window;
	HINSTANCE hInstance = NULL; // Handle to this application instance
	std::string myWindowTitle = "";
	std::wstring myWindowTitleWide = L"";
	std::string myWindowClass = "";
	std::wstring myWindowClassWide =  L"";
	int myWidth = 0;
	int myHeight = 0;

};