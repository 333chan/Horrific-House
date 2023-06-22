#pragma once

template<typename T>
void DebugMoveKey(unsigned int keyUp, unsigned int keyDown, int value, T& body)
{
	if (CheckHitKey(keyUp))
	{
		body += value;
	}
	if (CheckHitKey(keyDown))
	{
		body -= value;
	}
};


