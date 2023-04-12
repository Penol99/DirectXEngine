#include "KeyboardEvent.h"

KeyboardEvent::KeyboardEvent()
    :mType(EventType::Invalid),
    mKey(0u)
{
}

KeyboardEvent::KeyboardEvent(const EventType aType, const unsigned char aKey)
    :mType(aType),
    mKey(aKey)
{
}

bool KeyboardEvent::IsPress() const
{
    return mType == EventType::Press;
}

bool KeyboardEvent::IsRelease() const
{
    return mType == EventType::Release;
}
bool KeyboardEvent::IsValid() const
{
    return mType != EventType::Invalid;
}

unsigned char KeyboardEvent::GetKeyCode() const
{
    return mKey;
}
