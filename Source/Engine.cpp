#include "Engine.h"
#include <iostream>
#include "Timer.h"
Engine* Engine::mInstance = nullptr;

bool Engine::Init(HINSTANCE hInstance, std::string aWindowTitle, std::string aWindowClass, int aWidth, int aHeight)
{
	if (!this->mRenderWindow.Init(this, hInstance, aWindowTitle, aWindowClass, aWidth, aHeight))
	{
		return false;
	}
	if (!mGfx.Init(mRenderWindow.GetHWND(), aWidth, aHeight))
	{
		return false;
	}
	return true;
}

bool Engine::ProcessMessages()
{
	return this->mRenderWindow.ProcessMessages();
}

void Engine::Update(const float &aDeltaTime)
{
	mInput.Update();
	if (mInput.IsMouseButtonDown(VK_RBUTTON))
	{
		const float camRotSpeed = 2.f * aDeltaTime;
		float camSpeed = 5.f* aDeltaTime;
		mGfx.mCamera.AdjustRotation((float)mInput.GetMouseDelta().y * camRotSpeed, (float)mInput.GetMouseDelta().x * camRotSpeed, 0.f);
		if (mInput.IsKeyDown(VK_SHIFT))
		{
		    camSpeed *= 2;
		}
		if (mInput.IsKeyDown(VK_KEY_W))
		{
			mGfx.mCamera.AdjustPosition(mGfx.mCamera.GetForwardVector() * camSpeed);
		}
		if (mInput.IsKeyDown(VK_KEY_S))
		{
			mGfx.mCamera.AdjustPosition(mGfx.mCamera.GetBackVector() * camSpeed);
		}
		if (mInput.IsKeyDown(VK_KEY_D))
		{
			mGfx.mCamera.AdjustPosition(mGfx.mCamera.GetRightVector() * camSpeed);
		}
		if (mInput.IsKeyDown(VK_KEY_A))
		{
			mGfx.mCamera.AdjustPosition(mGfx.mCamera.GetLeftVector() * camSpeed);
		}
		if (mInput.IsKeyDown(VK_SPACE))
		{
			mGfx.mCamera.AdjustPosition(0, camSpeed, 0.f);
		}
		if (mInput.IsKeyDown(VK_CONTROL))
		{
			mGfx.mCamera.AdjustPosition(0, -camSpeed, 0.f);
		}
	}
}

void Engine::Render(const int &aFPS)
{
	mGfx.Render(aFPS);
}