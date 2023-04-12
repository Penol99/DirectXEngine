#include "Engine.h"

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

void Engine::Update()
{
    //while (!mInput.Keyboard().CharBufferIsEmpty())
    //{
    //    unsigned char ch = mInput.Keyboard().ReadChar();
    //    std::string outMsg = "Char: ";
    //    outMsg += ch;
    //    outMsg += "\n";
    //    OutputDebugStringA(outMsg.c_str());
    //}
    ////if (mInput.Keyboard().KeyIsPressed(VK_RETURN))
    ////{
    ////    OutputDebugStringA("CUM");
    //
    //MouseEvent mouseEvent = mInput.Mouse().ReadEvent();
    //while (!mInput.Mouse().EventBufferIsEmpty())
    //{
    //    //MouseEvent me = mInput.Mouse().ReadEvent();
    //    //if (me.GetType() == MouseEvent::EventType::RAW_MOVE)
    //    //{
    //    //    std::string x = "X: ";
    //    //    x += std::to_string(me.GetPosX());
    //    //    std::string y = " Y: ";

    //    //    y += std::to_string(me.GetPosY());
    //    //    x += y;
    //    //    x += "\n";
    //    //    OutputDebugStringA(x.c_str());
    //    //}
    //}
}

void Engine::Render()
{
    mGfx.Render();
}