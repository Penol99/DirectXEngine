#include "KeyboardClass.h"

KeyboardClass::KeyboardClass()
{
    for (int i = 0; i < mKeyStates.count(); i++)
    {
        mKeyStates[i] = false;
    }
}

bool KeyboardClass::KeyIsPressed(const unsigned char aKeycode)
{
    return mKeyStates[aKeycode];
}

bool KeyboardClass::KeyBufferIsEmpty()
{
    return mKeyBuffer.empty();
}

bool KeyboardClass::CharBufferIsEmpty()
{
    return mCharBuffer.empty();
}

KeyboardEvent KeyboardClass::ReadKey()
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

unsigned char KeyboardClass::ReadChar()
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

void KeyboardClass::OnKeyPressed(const unsigned char aKey)
{
    mKeyStates[aKey] = true;
    mKeyBuffer.push(KeyboardEvent(KeyboardEvent::EventType::Press, aKey));
}

void KeyboardClass::OnKeyReleased(const unsigned char aKey)
{
    mKeyStates[aKey] = false;
    mKeyBuffer.push(KeyboardEvent(KeyboardEvent::EventType::Release, aKey));
}

void KeyboardClass::OnChar(const unsigned char aKey)
{
    mCharBuffer.push(aKey);
}

void KeyboardClass::EnableAutoRepeatKeys()
{
    mAutoRepeatKeys = true;
}

void KeyboardClass::DisableAutoRepeatChars()
{
    mAutoRepeatChars = true;

}

bool KeyboardClass::IsKeysAutoRepeat()
{
    return mAutoRepeatKeys;
}

bool KeyboardClass::IsCharsAutoRepeat()
{
    return mAutoRepeatChars;
}
