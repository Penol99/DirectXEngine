#include "KeyboardInput.h"

KeyboardInput::KeyboardInput()
{
    for (int i = 0; i < mKeyStates.count(); i++)
    {
        mKeyStates[i] = false;
    }
}

bool KeyboardInput::KeyIsPressed(const unsigned char aKeycode)
{
    return mKeyStates[aKeycode];
}

bool KeyboardInput::KeyBufferIsEmpty()
{
    return mKeyBuffer.empty();
}

bool KeyboardInput::CharBufferIsEmpty()
{
    return mCharBuffer.empty();
}

KeyboardEvent KeyboardInput::ReadKey()
{
    if (mKeyBuffer.empty())
    {
        return KeyboardEvent();
    }
    else
    {
        KeyboardEvent e = mKeyBuffer.front();
        mKeyBuffer.pop();
        return e;
    }

}

unsigned char KeyboardInput::ReadChar()
{
    if (mCharBuffer.empty())
    {
        return 0u;
    }
    else
    {
        unsigned char e = mCharBuffer.front();
        mCharBuffer.pop();
        return e;
    }
    return 0;
}

void KeyboardInput::OnKeyPressed(const unsigned char aKey)
{
    mKeyStates[aKey] = true;
    mKeyBuffer.push(KeyboardEvent(KeyboardEvent::EventType::Press, aKey));
}

void KeyboardInput::OnKeyReleased(const unsigned char aKey)
{
    mKeyStates[aKey] = false;
    mKeyBuffer.push(KeyboardEvent(KeyboardEvent::EventType::Release, aKey));
}

void KeyboardInput::OnChar(const unsigned char aKey)
{
    mCharBuffer.push(aKey);
}

void KeyboardInput::EnableAutoRepeatKeys()
{
    mAutoRepeatKeys = true;
}

void KeyboardInput::DisableAutoRepeatChars()
{
    mAutoRepeatChars = true;

}

bool KeyboardInput::IsKeysAutoRepeat()
{
    return mAutoRepeatKeys;
}

bool KeyboardInput::IsCharsAutoRepeat()
{
    return mAutoRepeatChars;
}
