#pragma once
#include "WindowContainer.h"
class Engine : WindowContainer
{
public:
	bool Init(HINSTANCE hInstance, std::string aWindowTitle, std::string aWindowClass, int aWidth, int aHeight);
	bool ProcessMessages();

	void Update(const float& aDeltaTime);
	void Render(const int& aFPS, const float& aDeltaTime);
	

	static inline Engine* GetInstance()
	{
		if (myInstance == nullptr)
		{
			myInstance = new Engine();
		}
		return myInstance;
	}
	static inline void DestroyInstance()
	{
		delete myInstance;
	}
private:
	static Engine* myInstance;
	Engine() = default;
};