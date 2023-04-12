#pragma once
#include "KeyboardEvent.h"
#include <queue>
#include <bitset>
class KeyboardInput
{
public:
	KeyboardInput();
	bool KeyIsPressed(const unsigned char aKeycode);
	bool KeyBufferIsEmpty();
	bool CharBufferIsEmpty();
	KeyboardEvent ReadKey();
	unsigned char ReadChar();

	bool IsKeysAutoRepeat();
	bool IsCharsAutoRepeat();

	friend class InputHandler;
private:
	bool mAutoRepeatKeys = false;
	bool mAutoRepeatChars = false;
	std::bitset<256> mKeyStates;
	std::queue<KeyboardEvent> mKeyBuffer;
	std::queue<unsigned char> mCharBuffer;

	void OnKeyPressed(const unsigned char aKey);
	void OnKeyReleased(const unsigned char aKey);
	void OnChar(const unsigned char aKey);
	void EnableAutoRepeatKeys();
	void DisableAutoRepeatChars();
};