#include "MouseClass.h"

MouseClass::MouseClass()
	:mLeftIsDown(false),
	mRightIsDown(false),
	mMiddleIsDown(false),
	x(0),
	y(0)
{
}

void MouseClass::OnLeftPressed(int x, int y)
{
	mLeftIsDown = true;
	mEventBuffer.push(MouseEvent(MouseEvent::EventType::LPress, x, y));
}

void MouseClass::OnLeftReleased(int x, int y)
{
	mLeftIsDown = false;
	mEventBuffer.push(MouseEvent(MouseEvent::EventType::LRelease, x, y));
}

void MouseClass::OnRightPressed(int x, int y)
{
	mRightIsDown = true;
	mEventBuffer.push(MouseEvent(MouseEvent::EventType::RPress, x, y));
}

void MouseClass::OnRightReleased(int x, int y)
{
	mRightIsDown = false;
	mEventBuffer.push(MouseEvent(MouseEvent::EventType::RRelease, x, y));
}

void MouseClass::OnMiddlePressed(int x, int y)
{
	mMiddleIsDown = true;
	mEventBuffer.push(MouseEvent(MouseEvent::EventType::MPress, x, y));
}

void MouseClass::OnMiddleReleased(int x, int y)
{
	mMiddleIsDown = false;
	mEventBuffer.push(MouseEvent(MouseEvent::EventType::MPress, x, y));
}

void MouseClass::OnWheelUp(int x, int y)
{
	
	mEventBuffer.push(MouseEvent(MouseEvent::EventType::WheelUp, x, y));
}

void MouseClass::OnWheelDown(int x, int y)
{
	mEventBuffer.push(MouseEvent(MouseEvent::EventType::WheelDown, x, y));

}

void MouseClass::OnMouseMove(int x, int y)
{
	this->x = x;
	this->y = y;
	mEventBuffer.push(MouseEvent(MouseEvent::EventType::Move, x, y));
}

void MouseClass::OnMouseMoveRaw(int x, int y)
{
	mEventBuffer.push(MouseEvent(MouseEvent::EventType::RAW_MOVE, x, y));
}

bool MouseClass::IsLeftDown()
{
	return mLeftIsDown;
}

bool MouseClass::IsMiddleDown()
{
	return mMiddleIsDown;
}

bool MouseClass::IsRightDown()
{
	return mRightIsDown;
}

int MouseClass::GetPosX()
{
	return x;
}

int MouseClass::GetPosY()
{
	return y;
}

MousePoint MouseClass::GetPos()
{
	return {x,y};
}

bool MouseClass::EventBufferIsEmpty()
{
	return mEventBuffer.empty();
}

MouseEvent MouseClass::ReadEvent()
{
	if (mEventBuffer.empty())
	{
		return MouseEvent();
	}
	else
	{
		MouseEvent e = mEventBuffer.front();
		mEventBuffer.pop();
		return e;
	}

	return MouseEvent();
}
