#include "WindowContainer.h"
RenderWindow::~RenderWindow()
{
	if (this->mHandle != NULL)
	{
		UnregisterClass(this->mWindowClassWide.c_str(), this->hInstance);
		DestroyWindow(this->mHandle);

	}
}

bool RenderWindow::Initialize(WindowContainer* pWindowContainer, HINSTANCE hInstance, std::string aWindowTitle, std::string aWindowClass, int aWidth, int aHeight)
{
	this->hInstance = hInstance;
	this->mWidth = aWidth;
	this->mHeight = aHeight;
	this->mWindowTitle = aWindowTitle;
	this->mWindowTitleWide = StringConverter::StringToWide(this->mWindowTitle);
	this->mWindowClass = aWindowClass;
	this->mWindowClassWide = StringConverter::StringToWide(this->mWindowClass);

	this->RegisterWindowClass();
	this->mHandle = CreateWindowEx
	(
		0, 
		mWindowClassWide.c_str(), 
		mWindowTitleWide.c_str(), 
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, 
		0, 
		0,
		mWidth, 
		mHeight, 
		NULL, 
		NULL, 
		this->hInstance, 
		pWindowContainer
	);

	if (this->mHandle == NULL)
	{
		ErrorLogger::Log(GetLastError(), "Failed to Create window with CreateWindowEX: " + this->mWindowTitle);
		return false;
	}
	ShowWindow(this->mHandle, SW_SHOW);
	SetForegroundWindow(this->mHandle);
	SetFocus(this->mHandle);
	return true;
}

bool RenderWindow::ProcessMessages()
{
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));

	while (PeekMessage(&msg, this->mHandle, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	if (msg.message == WM_NULL)
	{
		if (!IsWindow(this->mHandle))
		{
			this->mHandle = NULL;
			UnregisterClass(this->mWindowClassWide.c_str(), this->hInstance);
			return false;
		}
	}
	return true;
}

HWND RenderWindow::GetHWND() const
{
	return mHandle;
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
			ErrorLogger::Log("Error: Pointer to window container is null during WM_NCCREATE.");
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
	wc.lpszClassName = this->mWindowClassWide.c_str();
	wc.cbSize = sizeof(WNDCLASSEX);
	RegisterClassEx(&wc);
}
