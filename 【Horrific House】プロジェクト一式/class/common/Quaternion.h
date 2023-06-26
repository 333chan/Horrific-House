#pragma once
#include <DxLib.h>

struct Quaternion
{
    double w;
    double x;
    double y;
    double z;

    Quaternion();
    Quaternion(VECTOR rad);
    Quaternion(double w, double x, double y, double z);

    // �I�C���[�p����N�H�[�^�j�I���֕ϊ�
    static Quaternion Euler(VECTOR rad);
    static Quaternion Euler(double radX, double radY, double radZ);

    // �N�H�[�^�j�I���̍���
    static Quaternion Mult(Quaternion q1, Quaternion q2);
    Quaternion Mult(Quaternion q);

    // ���W����]������
    VECTOR PosAxis(Quaternion q, VECTOR pos);
    VECTOR PosAxis(VECTOR pos);

    // �N�H�[�^�j�I������I�C���[�p�֕ϊ�
    VECTOR ToEuler(Quaternion q);
    VECTOR ToEuler(void);

    // �N�H�[�^�j�I������s��֕ϊ�
    MATRIX ToMatrix(Quaternion q);
    MATRIX ToMatrix(void);

    // ��{�x�N�g�����擾
    VECTOR GetForward(void);
    VECTOR GetBack(void);
    VECTOR GetRight(void);
    VECTOR GetLeft(void);
    VECTOR GetUp(void);
    VECTOR GetDown(void);

    double Dot(Quaternion b);

    // ���K��
    Quaternion Normalized(void);
    void Normalize(void);

    // �t�N�H�[�^�j�I��
    Quaternion Inverse(void);

    // ���ʕ��
    Quaternion Slerp(Quaternion to, double t);

    // �Q�̃x�N�g���Ԃ̉�]�ʂ��擾����
    Quaternion RotateTowards(Quaternion from, Quaternion to, float maxDegreesDelta);
    Quaternion SlerpUnclamped(Quaternion& a, Quaternion& b, float t);
    Quaternion Identity(void);

    // �Ώە����̉�]
    double Length(void);
    double LengthSquared(void);
    VECTOR xyz(void);
    Quaternion Normalize(Quaternion q);
    void Normalize(Quaternion q, Quaternion& result);

    // ��{�x�N�g�����擾
    VECTOR GetDir(VECTOR dir);

    Quaternion operator*(float& rhs);
    const Quaternion operator*(const float& rhs);
    Quaternion operator+(Quaternion& rhs);
    const Quaternion operator+(const Quaternion& rhs);
};

// �w�莲���w��p���A��]������
Quaternion AngleAxis(double rad, VECTOR axis);

//�p�x�����o��
double AngleDifference(Quaternion q1, Quaternion q2);

//����
double Dot(Quaternion q1, Quaternion q2);