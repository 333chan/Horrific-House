#pragma once
#include <DxLib.h>
#include <string>
#include "Quaternion.h"
#include "Geometry.h"

struct Transform
{
	// ���f���̃n���h��ID
	int modelHandle_ = -1;

	// �傫��
	Vector3 scale_;
	// ��]
	Vector3 rot_;
	// �ʒu
	Vector3 pos_;

	MATRIX matScale_ = {};
	MATRIX matRot_ = {};
	MATRIX matPos_ = {};

	// ��]
	Quaternion quaRot_;

	// ���[�J����]
	Quaternion quaRotLocal_;

	/// <summary>
	/// ������
	/// </summary>
	/// <param name="modelName">���f���̖��O</param>
	/// <param name="pos">�ʒu</param>
	/// <param name="scale">�傫��</param>
	/// <param name="quaRot">��]</param>
	/// <param name="quaRotLocal">���[�J����]</param>
	void Init(std::string modelName, Vector3 pos, Vector3 scale, Vector3 quaRot, Vector3 quaRotLocal);

	// �X�V
	void Update(void);

	//�`��
	void Draw(void);

	//�j��
	void Release(void);

	// �O���������擾
	VECTOR GetForward(void);

	// ����������擾
	VECTOR GetBack(void);

	// �E�������擾
	VECTOR GetRight(void);

	// ���������擾
	VECTOR GetLeft(void);

	// ��������擾
	VECTOR GetUp(void);

	// ���������擾
	VECTOR GetDown(void);

	// �Ώە������擾
	VECTOR GetDir(VECTOR vec);
};

