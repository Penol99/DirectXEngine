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
		if (mInstance == nullptr)
		{
			mInstance = new Engine();
		}
		return mInstance;
	}
	static inline void DestroyInstance()
	{
		delete mInstance;
	}
private:
	static Engine* mInstance;
	Engine() = default;
};