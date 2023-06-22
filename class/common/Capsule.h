#pragma once
#include <DxLib.h>

struct Capsule
{
	Capsule() :topPos(VGet(0.0f, 0.0f, 0.0f)), downPos(VGet(0.0f, 0.0f, 0.0f)), radius(0.0f) {};
	Capsule(VECTOR topPos, VECTOR downPos, float r) :topPos(topPos), downPos(downPos), radius(r) {};

	// �eTransform����̑��Έʒu(�㑤)
	VECTOR topPos;

	// �eTransform����̑��Έʒu(����)
	VECTOR downPos;

	// ���a
	float radius;

	// �`��
	void Draw(void);

	// ����
	float GetHeight(void);

	// �J�v�Z���̒��S���W
	VECTOR GetCenter(void);
};
