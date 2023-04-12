#include "WindowContainer.h"
#include <iostream>
WindowContainer::WindowContainer()
{
	static bool rawInputInitd = false;
	if (!rawInputInitd)
	{
		RAWINPUTDEVICE rid;
		rid.usUsagePage = 0x01; // Mouse
		rid.usUsage = 0x02;
		rid.dwFlags = 0;
		rid.hwndTarget = NULL;

		if (RegisterRawInputDevices(&rid, 1,sizeof(rid)) == FALSE)
		{
			ErrorLog::Log(GetLastError(), "fucked up registering raw input devices lol.");
			exit(-1);
		}
		rawInputInitd = true;
	}
}

LRESULT WindowContainer::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (mInput.UpdateEvents(hwnd, uMsg, wParam, lParam))
	{
		DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	
    return DefWindowProc(hwnd,uMsg,wParam,lParam);
}
