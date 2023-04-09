#include "Engine.h"

bool Engine::Initialize(HINSTANCE hInstance, std::string aWindowTitle, std::string aWindowClass, int aWidth, int aHeight)
{
    if (!this->mRenderWindow.Initialize(this, hInstance, aWindowTitle, aWindowClass, aWidth, aHeight))
    {
        return false;
    }
    if (!mGfx.Initialize(mRenderWindow.GetHWND(), aWidth, aHeight))
    {
        return false;
    }
    return true;
}

bool Engine::ProcessMessages()
{
    return this->mRenderWindow.ProcessMessages();
}

void Engine::Update()
{
    while (!mKeyboard.CharBufferIsEmpty())
    {
        unsigned char ch = mKeyboard.ReadChar();
        std::string outMsg = "Char: ";
        outMsg += ch;
        outMsg += "\n";
        OutputDebugStringA(outMsg.c_str());
    }
    while (!mMouse.EventBufferIsEmpty())
    {
        MouseEvent me = mMouse.ReadEvent();
        //if (me.GetType() == MouseEvent::EventType::RAW_MOVE)
        //{
        //    std::string x = "X: ";
        //    x += std::to_string(me.GetPosX());
        //    std::string y = " Y: ";

        //    y += std::to_string(me.GetPosY());
        //    x += y;
        //    x += "\n";
        //    OutputDebugStringA(x.c_str());
        //}
    }
}

void Engine::RenderFrame()
{
    mGfx.RenderFrame();
}
