#include "Engine.h"
#include <Windows.h>
#include "Timer.h"

Timer Time;
void Run(void);
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,_In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{

	// Open Visual Studio console Window
	AllocConsole();
	FILE* fp;
	freopen_s(&fp, "CONOUT$", "w", stdout);


	HRESULT hr = CoInitialize(NULL);
	if (FAILED(hr))
	{
		ErrorLog::Log(hr, "fucked up calling CoInitialize");
		return -1;
	}

	Engine& engine = *Engine::GetInstance();
	if(engine.Init(hInstance, "Engine++", "Window", 1200, 1000))
	{
		Run();
	}
	return 0;
}

void Run()
{
	Engine& engine = *Engine::GetInstance();

	while (engine.ProcessMessages())
	{
		Time.Update();
		engine.Update(Time.GetDeltaTime());

		engine.Render(Time.GetFPS());
	}
}

