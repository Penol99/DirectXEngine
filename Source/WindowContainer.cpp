#include "WindowContainer.h"
#include <iostream>
WindowContainer::WindowContainer()
{
	static bool rawInputInit = false;
	if (!rawInputInit)
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
		rawInputInit = true;
	}
}
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT WindowContainer::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hwnd, uMsg, wParam, lParam))
	{
		return true;
	}
	return mInput.UpdateEvents(hwnd, uMsg, wParam, lParam);
}
