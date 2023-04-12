#include "MouseInput.h"

MouseInput::MouseInput()
	:mLeftIsDown(false),
	mRightIsDown(false),
	mMiddleIsDown(false),
	x(0),
	y(0)
{
}

void MouseInput::OnLeftPressed(int x, int y)
{
	mLeftIsDown = true;
	mEventBuffer.push(MouseEvent(MouseEvent::EventType::LPress, x, y));
}

void MouseInput::OnLeftReleased(int x, int y)
{
	mLeftIsDown = false;
	mEventBuffer.push(MouseEvent(MouseEvent::EventType::LRelease, x, y));
}

void MouseInput::OnRightPressed(int x, int y)
{
	mRightIsDown = true;
	mEventBuffer.push(MouseEvent(MouseEvent::EventType::RPress, x, y));
}

void MouseInput::OnRightReleased(int x, int y)
{
	mRightIsDown = false;
	mEventBuffer.push(MouseEvent(MouseEvent::EventType::RRelease, x, y));
}

void MouseInput::OnMiddlePressed(int x, int y)
{
	mMiddleIsDown = true;
	mEventBuffer.push(MouseEvent(MouseEvent::EventType::MPress, x, y));
}

void MouseInput::OnMiddleReleased(int x, int y)
{
	mMiddleIsDown = false;
	mEventBuffer.push(MouseEvent(MouseEvent::EventType::MPress, x, y));
}

void MouseInput::OnWheelUp(int x, int y)
{
	
	mEventBuffer.push(MouseEvent(MouseEvent::EventType::WheelUp, x, y));
}

void MouseInput::OnWheelDown(int x, int y)
{
	mEventBuffer.push(MouseEvent(MouseEvent::EventType::WheelDown, x, y));

}

void MouseInput::OnMouseMove(int x, int y)
{
	this->x = x;
	this->y = y;
	mEventBuffer.push(MouseEvent(MouseEvent::EventType::Move, x, y));
}

void MouseInput::OnMouseMoveRaw(int x, int y)
{
	mEventBuffer.push(MouseEvent(MouseEvent::EventType::RAW_MOVE, x, y));
}

bool MouseInput::IsLeftDown()
{
	return mLeftIsDown;
}

bool MouseInput::IsMiddleDown()
{
	return mMiddleIsDown;
}

bool MouseInput::IsRightDown()
{
	return mRightIsDown;
}

int MouseInput::GetPosX()
{
	return x;
}

int MouseInput::GetPosY()
{
	return y;
}

MousePoint MouseInput::GetPos()
{
	return {x,y};
}

bool MouseInput::EventBufferIsEmpty()
{
	return mEventBuffer.empty();
}

MouseEvent MouseInput::ReadEvent()
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
