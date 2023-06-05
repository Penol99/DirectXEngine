#include "WindowContainer.h"


RenderWindow::~RenderWindow()
{
	if (this->myHandle != NULL)
	{
		UnregisterClass(this->myWindowClassWide.c_str(), this->hInstance);
		DestroyWindow(this->myHandle);

	}
}

bool RenderWindow::Init(Graphics& aGFXObject, WindowContainer* pWindowContainer, HINSTANCE hInstance, std::string aWindowTitle, std::string aWindowClass, int aWidth, int aHeight)
{

	myGfx = &aGFXObject;
	hInstance = hInstance;

	myWindowTitle = aWindowTitle;
	myWindowTitleWide = StringConverter::StringToWide(this->myWindowTitle);
	myWindowClass = aWindowClass;
	myWindowClassWide = StringConverter::StringToWide(this->myWindowClass);

	this->RegisterWindowClass();


	RECT windowRect;
	windowRect.left = GetSystemMetrics(SM_CXSCREEN) / 2 - aWidth / 2;
	windowRect.top = GetSystemMetrics(SM_CYSCREEN) / 2 - aHeight / 2;
	windowRect.right = windowRect.left + aWidth;
	windowRect.bottom = windowRect.top + aHeight;

	AdjustWindowRect(&windowRect, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU | WS_MAXIMIZEBOX, FALSE);


	this->myHandle = CreateWindowEx
	(
		0,
		myWindowClassWide.c_str(),
		myWindowTitleWide.c_str(),
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,
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


	UINT dpi = GetDpiForWindow(myHandle);
	myScaleFactor = (float)dpi / 96.0f;

	myWidth = (int)(aWidth * myScaleFactor);
	myHeight = (int)(aHeight * myScaleFactor);

	ShowWindow(this->myHandle, SW_SHOW);
	SetForegroundWindow(this->myHandle);
	SetFocus(this->myHandle);

	//SetFullscreen(false, myHandle);

	// Adjust the window position and size to cover the entire screen
	int screenWidth = GetSystemMetricsForDpi(SM_CXSCREEN, dpi);
	int screenHeight = GetSystemMetricsForDpi(SM_CYSCREEN, dpi);
	SetWindowPos(myHandle, NULL, 0, 0, screenWidth, screenHeight, SWP_FRAMECHANGED);
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

void RenderWindow::SetFullscreen(bool aValue, HWND& aHwnd)
{
	static WINDOWPLACEMENT windowPlacement = { sizeof(WINDOWPLACEMENT) };
	static LONG_PTR savedWindowStyle = 0;
	static bool firstTime = true;

	myIsFullscreen = aValue;

	// Get the monitor info
	MONITORINFO mi = { sizeof(mi) };
	if (GetMonitorInfo(MonitorFromWindow(aHwnd, MONITOR_DEFAULTTOPRIMARY), &mi))
	{
		if (myIsFullscreen)
		{
			if (firstTime)
			{
				// Save the current window style and position
				savedWindowStyle = GetWindowLongPtr(aHwnd, GWL_STYLE);
				GetWindowPlacement(aHwnd, &windowPlacement);
				firstTime = false;
			}

			// Make the window fill the available screen space, preserving the taskbar and title bar
			ShowWindow(aHwnd, SW_MAXIMIZE);
		}
		else
		{
			// Restore the previous window style and position
			SetWindowLongPtr(aHwnd, GWL_STYLE, savedWindowStyle);
			SetWindowPlacement(aHwnd, &windowPlacement);

			// Make the windowed mode to be of regular size
			SetWindowPos(aHwnd, NULL, windowPlacement.rcNormalPosition.left, windowPlacement.rcNormalPosition.top,
				windowPlacement.rcNormalPosition.right - windowPlacement.rcNormalPosition.left,
				windowPlacement.rcNormalPosition.bottom - windowPlacement.rcNormalPosition.top,
				SWP_FRAMECHANGED | SWP_NOZORDER | SWP_NOOWNERZORDER);
		}
	}

	RECT rect;
	GetClientRect(aHwnd, &rect);
	int width = rect.right - rect.left;
	int height = rect.bottom - rect.top;
	myGfx->Resize(width,height);

	std::cout << "Width: " << width << " Height: " << height << std::endl;
}

HWND RenderWindow::GetHWND() const
{
	return myHandle;
}

float RenderWindow::GetScaleFactor()
{
	return myScaleFactor;
}

LRESULT CALLBACK HandleMsgRedirect(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CLOSE:
	{
		DestroyWindow(hwnd);
		return 0;
	}
	case WM_SYSCOMMAND:
	{
		switch (wParam & 0xfff0)  // Extract the system command code
		{
		case SC_MAXIMIZE:  // Window is being maximized
		{
			RenderWindow::SetFullscreen(true, hwnd);
			break;
		}
		case SC_RESTORE:  // Window is being restored
		{
			if (RenderWindow::GetIsFullscreen())
			{
				RenderWindow::SetFullscreen(false, hwnd);
			}
			break;
		}
		}
		break;
	}
	case WM_SIZE:
	{
		if (ImGui::GetCurrentContext() != NULL)
		{
			ImGuiIO& io = ImGui::GetIO();
			io.DisplaySize.x = (float)LOWORD(lParam);
			io.DisplaySize.y = (float)HIWORD(lParam);
		}
		break;
	}
	default:
	{
		WindowContainer* const pWindow = reinterpret_cast<WindowContainer*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
		return pWindow->WindowProc(hwnd, uMsg, wParam, lParam);
	}
	}
	// handle default behavior for messages not handled in switch-case
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
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
