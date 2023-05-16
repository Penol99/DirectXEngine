#include "Engine.h"
#include <iostream>
#include "Timer.h"
Engine* Engine::myInstance = nullptr;

bool Engine::Init(HINSTANCE hInstance, std::string aWindowTitle, std::string aWindowClass, int aWidth, int aHeight, Timer& aTimer)
{
	if (!this->myRenderWindow.Init(this, hInstance, aWindowTitle, aWindowClass, aWidth, aHeight))
	{
		return false;
	}
	if (!myGfx.Init(myRenderWindow.GetHWND(), aWidth, aHeight, aTimer))
	{
		return false;
	}

	return true;
}

bool Engine::ProcessMessages()
{
	return this->myRenderWindow.ProcessMessages();
}

void Engine::Update(const float &aDeltaTime)
{
	myInput.Update();
	if (myInput.IsMouseButtonDown(VK_RBUTTON))
	{
		const float camRotSpeed = 2.f * aDeltaTime;
		myGfx.myCamera.AdjustRotation((float)myInput.GetMouseDelta().y * camRotSpeed, (float)myInput.GetMouseDelta().x * camRotSpeed, 0.f);
		float camSpeed = myGfx.myCameraSpeed*aDeltaTime;
		if (myInput.IsKeyDown(VK_SHIFT))
		{
		    camSpeed *= 2;
		}
		if (myInput.IsKeyDown(VK_KEY_W))
		{
			myGfx.myCamera.AdjustPosition(myGfx.myCamera.GetForwardVector() * camSpeed);
		}
		if (myInput.IsKeyDown(VK_KEY_S))
		{
			myGfx.myCamera.AdjustPosition(myGfx.myCamera.GetBackVector() * camSpeed);
		}
		if (myInput.IsKeyDown(VK_KEY_D))
		{
			myGfx.myCamera.AdjustPosition(myGfx.myCamera.GetRightVector() * camSpeed);
		}
		if (myInput.IsKeyDown(VK_KEY_A))
		{
			myGfx.myCamera.AdjustPosition(myGfx.myCamera.GetLeftVector() * camSpeed);
		}
		if (myInput.IsKeyDown(VK_SPACE))
		{
			myGfx.myCamera.AdjustPosition(0, camSpeed , 0.f);
		}
		if (myInput.IsKeyDown(VK_CONTROL))
		{
			myGfx.myCamera.AdjustPosition(0, -camSpeed, 0.f);
		}
	}
}

void Engine::Render(const int &aFPS, const float& aDeltaTime)
{


	myGfx.Render(aFPS,aDeltaTime);

}