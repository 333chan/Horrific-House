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

    // オイラー角からクォータニオンへ変換
    static Quaternion Euler(VECTOR rad);
    static Quaternion Euler(double radX, double radY, double radZ);

    // クォータニオンの合成
    static Quaternion Mult(Quaternion q1, Quaternion q2);
    Quaternion Mult(Quaternion q);

    // 座標を回転させる
    VECTOR PosAxis(Quaternion q, VECTOR pos);
    VECTOR PosAxis(VECTOR pos);

    // クォータニオンからオイラー角へ変換
    VECTOR ToEuler(Quaternion q);
    VECTOR ToEuler(void);

    // クォータニオンから行列へ変換
    MATRIX ToMatrix(Quaternion q);
    MATRIX ToMatrix(void);

    // 基本ベクトルを取得
    VECTOR GetForward(void);
    VECTOR GetBack(void);
    VECTOR GetRight(void);
    VECTOR GetLeft(void);
    VECTOR GetUp(void);
    VECTOR GetDown(void);

    double Dot(Quaternion b);

    // 正規化
    Quaternion Normalized(void);
    void Normalize(void);

    // 逆クォータニオン
    Quaternion Inverse(void);

    // 球面補間
    Quaternion Slerp(Quaternion to, double t);

    // ２つのベクトル間の回転量を取得する
    Quaternion RotateTowards(Quaternion from, Quaternion to, float maxDegreesDelta);
    Quaternion SlerpUnclamped(Quaternion& a, Quaternion& b, float t);
    Quaternion Identity(void);

    // 対象方向の回転
    double Length(void);
    double LengthSquared(void);
    VECTOR xyz(void);
    Quaternion Normalize(Quaternion q);
    void Normalize(Quaternion q, Quaternion& result);

    // 基本ベクトルを取得
    VECTOR GetDir(VECTOR dir);

    Quaternion operator*(float& rhs);
    const Quaternion operator*(const float& rhs);
    Quaternion operator+(Quaternion& rhs);
    const Quaternion operator+(const Quaternion& rhs);
};

// 指定軸を指定角分、回転させる
Quaternion AngleAxis(double rad, VECTOR axis);

//角度差を出す
double AngleDifference(Quaternion q1, Quaternion q2);

//内積
double Dot(Quaternion q1, Quaternion q2);