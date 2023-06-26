#include <cmath>
#include <DxLib.h>
#include "Geometry.h"
#include "Quaternion.h"

// •ûŒü
constexpr VECTOR DIR_F = { 0.0f, 0.0f, 1.0f };
constexpr VECTOR DIR_B = { 0.0f, 0.0f, -1.0f };
constexpr VECTOR DIR_R = { 1.0f, 0.0f, 0.0f };
constexpr VECTOR DIR_L = { -1.0f, 0.0f, 0.0f };
constexpr VECTOR DIR_U = { 0.0f, 1.0f, 0.0f };
constexpr VECTOR DIR_D = { 0.0f, -1.0f, 0.0f };

Quaternion::Quaternion()
{
    w = 1;
    x = y = z = 0;
}

Quaternion::Quaternion(VECTOR rad)
{
    Quaternion q = Euler(rad.x, rad.y, rad.z);
    w = q.w;
    x = q.x;
    y = q.y;
    z = q.z;
}

Quaternion::Quaternion(double ww, double wx, double wy, double wz)
{
    w = ww;
    x = wx;
    y = wy;
    z = wz;
}

Quaternion Quaternion::Euler(VECTOR rad)
{
    return Euler(rad.x, rad.y, rad.z);
}

Quaternion Quaternion::Euler(double radX, double radY, double radZ)
{
    Quaternion ret = Quaternion();

    radX = RadIn2PI(radX);
    radY = RadIn2PI(radY);
    radZ = RadIn2PI(radZ);

    double cosZ = cos(radZ / 2.0f);
    double sinZ = sin(radZ / 2.0f);
    double cosX = cos(radX / 2.0f);
    double sinX = sin(radX / 2.0f);
    double cosY = cos(radY / 2.0f);
    double sinY = sin(radY / 2.0f);

    ret.w = cosX * cosY * cosZ + sinX * sinY * sinZ;
    ret.x = sinX * cosY * cosZ + cosX * sinY * sinZ;
    ret.y = cosX * sinY * cosZ - sinX * cosY * sinZ;
    ret.z = cosX * cosY * sinZ - sinX * sinY * cosZ;

    return ret;
}

Quaternion Quaternion::Mult(Quaternion q1, Quaternion q2)
{

    Quaternion ret = Quaternion();
    double d1, d2, d3, d4;

    // w‚ÌŒvŽZ 
    d1 = q1.w * q2.w;
    d2 = -q1.x * q2.x;
    d3 = -q1.y * q2.y;
    d4 = -q1.z * q2.z;
    ret.w = d1 + d2 + d3 + d4;

    // x‚ÌŒvŽZ 
    d1 = q1.w * q2.x;
    d2 = q2.w * q1.x;
    d3 = q1.y * q2.z;
    d4 = -q1.z * q2.y;
    ret.x = d1 + d2 + d3 + d4;

    // y‚ÌŒvŽZ
    d1 = q1.w * q2.y;
    d2 = q2.w * q1.y;
    d3 = q1.z * q2.x;
    d4 = -q1.x * q2.z;
    ret.y = d1 + d2 + d3 + d4;

    // z‚ÌŒvŽZ
    d1 = q1.w * q2.z;
    d2 = q2.w * q1.z;
    d3 = q1.x * q2.y;
    d4 = -q1.y * q2.x;
    ret.z = d1 + d2 + d3 + d4;

    return ret;

}

Quaternion Quaternion::Mult(Quaternion q)
{
    return Mult(Quaternion(w, x, y, z), q);
}

VECTOR Quaternion::PosAxis(Quaternion q, VECTOR pos)
{
    // ˆÊ’uî•ñ‚É‰ñ“]î•ñ‚ð”½‰f‚³‚¹‚é
    Quaternion tmp = Quaternion();
    tmp = tmp.Mult(q);
    tmp = tmp.Mult(Quaternion(0.0f, pos.x, pos.y, pos.z));
    tmp = tmp.Mult(q.Inverse());
    return { (float)tmp.x, (float)tmp.y, (float)tmp.z };
}

VECTOR Quaternion::PosAxis(VECTOR pos)
{
    return PosAxis(Quaternion(w, x, y, z), pos);
}

VECTOR Quaternion::ToEuler(Quaternion q)
{

    VECTOR ret;

    double r11 = 2 * (q.x * q.z + q.w * q.y);
    double r12 = q.w * q.w - q.x * q.x - q.y * q.y + q.z * q.z;
    double r21 = -2 * (q.y * q.z - q.w * q.x);
    double r31 = 2 * (q.x * q.y + q.w * q.z);
    double r32 = q.w * q.w - q.x * q.x + q.y * q.y - q.z * q.z;

    ret.x = (float)asin(r21);
    ret.y = (float)atan2(r11, r12);
    ret.z = (float)atan2(r31, r32);

    return ret;

}

VECTOR Quaternion::ToEuler(void)
{
    return ToEuler(Quaternion(w, x, y, z));
}

MATRIX Quaternion::ToMatrix(Quaternion q)
{

    float sqw = (float)(q.w * q.w);
    float sqx = (float)(q.x * q.x);
    float sqy = (float)(q.y * q.y);
    float sqz = (float)(q.z * q.z);
    float invs = 1.0f / (sqx + sqy + sqz + sqw);

    MATRIX matrix = MGetIdent();

    matrix.m[0][0] = (sqx - sqy - sqz + sqw) * invs;
    matrix.m[1][1] = (-sqx + sqy - sqz + sqw) * invs;
    matrix.m[2][2] = (-sqx - sqy + sqz + sqw) * invs;

    float tmp1 = (float)(q.x * q.y);
    float tmp2 = (float)(q.z * q.w);
    matrix.m[0][1] = 2.0f * (tmp1 + tmp2) * invs;
    matrix.m[1][0] = 2.0f * (tmp1 - tmp2) * invs;

    tmp1 = (float)(q.x * q.z);
    tmp2 = (float)(q.y * q.w);
    matrix.m[0][2] = 2.0f * (tmp1 - tmp2) * invs;
    matrix.m[2][0] = 2.0f * (tmp1 + tmp2) * invs;

    tmp1 = (float)(q.y * q.z);
    tmp2 = (float)(q.x * q.w);
    matrix.m[1][2] = 2.0f * (tmp1 + tmp2) * invs;
    matrix.m[2][1] = 2.0f * (tmp1 - tmp2) * invs;

    return matrix;
}

MATRIX Quaternion::ToMatrix(void)
{
    return ToMatrix(Quaternion(w, x, y, z));
}

VECTOR Quaternion::GetDir(VECTOR dir)
{
    return PosAxis(dir);
}

VECTOR Quaternion::GetForward(void)
{
    return GetDir(DIR_F);
}

VECTOR Quaternion::GetBack(void)
{
    return GetDir(DIR_B);
}

VECTOR Quaternion::GetRight(void)
{
    return GetDir(DIR_R);
}

VECTOR Quaternion::GetLeft(void)
{
    return GetDir(DIR_L);
}

VECTOR Quaternion::GetUp(void)
{
    return GetDir(DIR_U);
}

VECTOR Quaternion::GetDown(void)
{
    return GetDir(DIR_D);
}

double Quaternion::Dot(Quaternion q)
{
    return (w * q.w + x * q.x + y * q.y + z * q.z);
}

Quaternion Quaternion::Normalized(void)
{

    double mag = sqrt(w * w + x * x + y * y + z * z);
    return Quaternion(w / mag, x / mag, y / mag, z / mag);

}

void Quaternion::Normalize(void)
{

    double mag = sqrt(w * w + x * x + y * y + z * z);

    w /= mag;
    x /= mag;
    y /= mag;
    z /= mag;
}

Quaternion Quaternion::Inverse(void)
{

    double n = 1.0f / (w * w + x * x + y * y + z * z);
    Quaternion tmp = Quaternion(w, -x, -y, -z);
    return Quaternion(tmp.w * n, tmp.x * n, tmp.y * n, tmp.z * n);;
}

Quaternion Quaternion::Slerp(Quaternion to, double t)
{
    if (t > 1.0)
    {
        t = 1.0;
    }
    if (t < 0.0)
    {
        t = 0,0;
    }
    Quaternion from = Quaternion(w, x, y, z);

    return SlerpUnclamped(from, to, (float)t);
}

Quaternion Quaternion::RotateTowards(Quaternion from, Quaternion to, float maxDegreesDelta)
{
    double num = AngleDifference(from, to);
    if (num == 0.0)
    {
        return to;
    }
    float t = min(1.0f, maxDegreesDelta / (float)num);
    return Quaternion::SlerpUnclamped(from, to, t);
}

Quaternion Quaternion::SlerpUnclamped(Quaternion& q1, Quaternion& q2, float t)
{
    if (q1.LengthSquared() == 0.0f)
    {
        if (q2.LengthSquared() == 0.0f)
        {
            return Identity();
        }
        return q2;
    }
    else if (q2.LengthSquared() == 0.0f)
    {
        return q1;
    }

    float cosHalfAngle = (float)(q1.w * q2.w) + VDot(q1.xyz(), q2.xyz());

    if (cosHalfAngle >= 1.0f || cosHalfAngle <= -1.0f)
    {
        return q1;
    }
    else if (cosHalfAngle < 0.0f)
    {
        //b.xyz() = -b.xyz();
		q2.x = q2.x * -1.0f;
		q2.y = q2.y * -1.0f;
		q2.z = q2.z * -1.0f;
		q2.w = -q2.w;
        cosHalfAngle = -cosHalfAngle;
    }

    float blendA;
    float blendB;
    if (cosHalfAngle < 0.99f)
    {
        float halfAngle = acosf(cosHalfAngle);
        float sinHalfAngle = sinf(halfAngle);
        float oneOverSinHalfAngle = 1.0f / sinHalfAngle;
        blendA = sinf(halfAngle * (1.0f - t)) * oneOverSinHalfAngle;
        blendB = sinf(halfAngle * t) * oneOverSinHalfAngle;
    }
    else
    {
        blendA = 1.0f - t;
        blendB = t;
    }

    VECTOR v = VAdd(VScale(q1.xyz(), blendA), VScale(q2.xyz(), blendB));
	Quaternion result = Quaternion(blendA * q1.w + blendB * q2.w, v.x, v.y, v.z);
    if (result.LengthSquared() > 0.0f)
    {
        return Normalize(result);
    }
    else
    {
        return Identity();
    }
}

Quaternion Quaternion::Identity(void)
{
    return Quaternion(1.0f, 0.0f, 0.0f, 0.0f);
}

double Quaternion::Length(void)
{
    return sqrt(x * x + y * y + z * z + w * w);
}

double Quaternion::LengthSquared(void)
{
    return x * x + y * y + z * z + w * w;
}

VECTOR Quaternion::xyz(void)
{
    return { (float)x, (float)y, (float)z };
}

Quaternion Quaternion::Normalize(Quaternion q)
{
    Quaternion result = Quaternion();
    Normalize(q, result);
    return result;
}

void Quaternion::Normalize(Quaternion q, Quaternion& result)
{

	float scale = 1.0f / (float)q.Length();
	VECTOR v = VScale(q.xyz(), scale);
	Quaternion ret = Quaternion(q.w * scale, v.x, v.y, v.z);
	result.x = ret.x;
	result.y = ret.y;
	result.z = ret.z;
	result.w = ret.w;
}

Quaternion Quaternion::operator*(float& f)
{
    return Quaternion(w * f, x * f, y * f, z * f);
}

const Quaternion Quaternion::operator*(const float& f) 
{
    return Quaternion(w * f, x * f, y * f, z * f);
}

Quaternion Quaternion::operator+(Quaternion& rhs) 
{
    return Quaternion(w + rhs.w, x + rhs.x, y + rhs.y, z + rhs.z);
}

const Quaternion Quaternion::operator+(const Quaternion& rhs)
{
    return Quaternion(w + rhs.w, x + rhs.x, y + rhs.y, z + rhs.z);
}

Quaternion AngleAxis(double rad, VECTOR axis)
{
    Quaternion ret = Quaternion();

    double norm;
    double c, s;

    ret.w = 1.0;
    ret.x = ret.y = ret.z = 0.0;

    norm = (double)axis.x * (double)axis.x + (double)axis.y * (double)axis.y + (double)axis.z * (double)axis.z;
    if (norm <= 0.0f)
    {
        return ret;
    }

    norm = 1.0 / sqrt(norm);
    axis.x = (float)(axis.x * norm);
    axis.y = (float)(axis.y * norm);
    axis.z = (float)(axis.z * norm);

    c = cos(0.5f * rad);
    s = sin(0.5f * rad);

    ret.w = c;
    ret.x = s * axis.x;
    ret.y = s * axis.y;
    ret.z = s * axis.z;

    return ret;
}

double AngleDifference(Quaternion q1, Quaternion q2)
{
    double cos = Dot(q1, q2);
    double ac = acos(cos);
    return ac * (180.0 / DX_PI);
}

double Dot(Quaternion q1, Quaternion q2)
{
    return (q1.w * q2.w + q1.x * q2.x + q1.y * q2.y + q1.z * q2.z);
}
