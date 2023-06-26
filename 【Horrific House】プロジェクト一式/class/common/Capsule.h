#pragma once
#include <DxLib.h>

struct Capsule
{
	Capsule() :topPos(VGet(0.0f, 0.0f, 0.0f)), downPos(VGet(0.0f, 0.0f, 0.0f)), radius(0.0f) {};
	Capsule(VECTOR topPos, VECTOR downPos, float r) :topPos(topPos), downPos(downPos), radius(r) {};

	// 親Transformからの相対位置(上側)
	VECTOR topPos;

	// 親Transformからの相対位置(下側)
	VECTOR downPos;

	// 半径
	float radius;

	// 描画
	void Draw(void);

	// 高さ
	float GetHeight(void);

	// カプセルの中心座標
	VECTOR GetCenter(void);
};
