#include "WindowContainer.h"


RenderWindow::~RenderWindow()
{
	if (this->myHandle != NULL)
	{
		UnregisterClass(this->myWindowClassWide.c_str(), this->hInstance);
		DestroyWindow(this->myHandle);

	}
}

bool RenderWindow::Init(WindowContainer* pWindowContainer, HINSTANCE hInstance, std::string aWindowTitle, std::string aWindowClass, int aWidth, int aHeight)
{
	this->hInstance = hInstance;
	this->myWidth = aWidth;
	this->myHeight = aHeight;
	this->myWindowTitle = aWindowTitle;
	this->myWindowTitleWide = StringConverter::StringToWide(this->myWindowTitle);
	this->myWindowClass = aWindowClass;
	this->myWindowClassWide = StringConverter::StringToWide(this->myWindowClass);

	this->RegisterWindowClass();


	RECT windowRect;
	windowRect.left = GetSystemMetrics(SM_CXSCREEN) / 2 - aWidth / 2;
	windowRect.top = GetSystemMetrics(SM_CYSCREEN) / 2 - aHeight / 2;
	windowRect.right = windowRect.left + aWidth;
	windowRect.bottom = windowRect.top + aHeight;

	AdjustWindowRect(&windowRect, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE);


	this->myHandle = CreateWindowEx
	(
		0,
		myWindowClassWide.c_str(),
		myWindowTitleWide.c_str(),
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
		windowRect.left, // Window X 
		windowRect.top, // Window Y 
		windowRect.right - windowRect.left, // Window Width
		windowRect.bottom - windowRect.top, // Window Height
		NULL,
		NULL,
		this->hInstance,
		pWindowContainer
	);

	if (this->myHandle == NULL)
	{
		ErrorLog::Log(GetLastError(), "Failed to Create window with CreateWindowEX: " + this->myWindowTitle);
		return false;
	}
	ShowWindow(this->myHandle, SW_SHOW);
	SetForegroundWindow(this->myHandle);
	SetFocus(this->myHandle);
	return true;
}

bool RenderWindow::ProcessMessages()
{
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));

	while (PeekMessage(&msg, this->myHandle, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	if (msg.message == WM_NULL)
	{
		if (!IsWindow(this->myHandle))
		{
			this->myHandle = NULL;
			UnregisterClass(this->myWindowClassWide.c_str(), this->hInstance);
			return false;
		}
	}
	return true;
}

HWND RenderWindow::GetHWND() const
{
	return myHandle;
}

LRESULT CALLBACK HandleMsgRedirect(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CLOSE:
		DestroyWindow(hwnd);
		return 0;
	default:
	{
		WindowContainer* const pWindow = reinterpret_cast<WindowContainer*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
		return pWindow->WindowProc(hwnd, uMsg, wParam, lParam);
		break;
	}
	}
}

LRESULT CALLBACK HandleMessageSetup(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_NCCREATE:
	{
		const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
		WindowContainer* pWindow = reinterpret_cast<WindowContainer*>(pCreate->lpCreateParams);
		if (pWindow == nullptr)
		{
			ErrorLog::Log("Error: Pointer to window container is null during WM_NCCREATE.");
			exit(-1);
		}
		SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWindow));
		SetWindowLongPtr(hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(HandleMsgRedirect));
		return pWindow->WindowProc(hwnd, uMsg, wParam, lParam);
		//return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
}

void RenderWindow::RegisterWindowClass()
{
	WNDCLASSEX wc;
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = HandleMessageSetup;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = this->hInstance;
	wc.hIcon = NULL;
	wc.hIconSm = NULL;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = NULL;
	wc.lpszMenuName = NULL;
	wc.lpszClassName = this->myWindowClassWide.c_str();
	wc.cbSize = sizeof(WNDCLASSEX);
	RegisterClassEx(&wc);
}
