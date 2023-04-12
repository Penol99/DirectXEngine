#pragma once
#include "MouseEvent.h"
#include <queue>
class MouseInput
{
public:
	
	bool IsLeftDown();
	bool IsMiddleDown();
	bool IsRightDown();

	int GetPosX();
	int GetPosY();

	MousePoint GetPos();

	bool EventBufferIsEmpty();
	MouseEvent ReadEvent();
	MouseInput();
	friend class InputHandler;
private:
	std::queue<MouseEvent> mEventBuffer;
	bool mLeftIsDown;
	bool mRightIsDown;
	bool mMiddleIsDown;
	int x;
	int y;

	void OnLeftPressed(int x, int y);
	void OnLeftReleased(int x, int y);
	void OnRightPressed(int x, int y);
	void OnRightReleased(int x, int y);
	void OnMiddlePressed(int x, int y);
	void OnMiddleReleased(int x, int y);
	void OnWheelUp(int x, int y);
	void OnWheelDown(int x, int y);
	void OnMouseMove(int x, int y);
	void OnMouseMoveRaw(int x, int y);
};