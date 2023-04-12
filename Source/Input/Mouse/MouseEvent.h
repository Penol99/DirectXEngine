#pragma once
struct MousePoint
{
	MousePoint(int aX, int aY)
		:x(aX),
		y(aY)
	{
	}
	int x;
	int y;
};
class MouseEvent
{
public:
	enum EventType
	{
		LPress,
		LRelease,
		RPress,
		RRelease,
		MPress,
		MRelease,
		WheelUp,
		WheelDown,
		Move,
		RAW_MOVE,
		Invalid,
	};
private:
	EventType mType;
	int mX;
	int mY;
public:
	MouseEvent();
	MouseEvent(const EventType aType, const int x, const int y);
	bool IsValid() const;
	EventType GetType() const;
	MousePoint GetPos() const;
	int GetPosX() const;
	int GetPosY() const;
};