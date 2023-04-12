#include "Engine.h"

void Run(void);
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,_In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	
	HRESULT hr = CoInitialize(NULL);
	if (FAILED(hr))
	{
		ErrorLog::Log(hr, "fucked up calling CoInitialize");
		return -1;
	}

	Engine& engine = *Engine::GetInstance();
	if(engine.Init(hInstance, "Engine++", "Window", 800, 600))
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
		engine.Update();

		engine.Render();
	}
}

