#include "MouseEvent.h"

MouseEvent::MouseEvent()
	:mType(MouseEvent::EventType::Invalid),
	mX(0),
	mY(0)
{
}

MouseEvent::MouseEvent(const EventType aType, const int x, const int y)
	: mType(aType),
	mX(x),
	mY(y)
{
}

bool MouseEvent::IsValid() const
{
	return mType != MouseEvent::EventType::Invalid;
}

MouseEvent::EventType MouseEvent::GetType() const
{
	return mType;
}

MousePoint MouseEvent::GetPos() const
{
	return { mX,mY };
}

int MouseEvent::GetPosX() const
{
	return mX;
}

int MouseEvent::GetPosY() const
{
	return mY;
}
