#include"Geometry.h"
#include<cmath>

void Vector2::operator*=(float scale)
{
	Vector2(x *= scale, y *= scale);
}

Vector2 Vector2::operator*(float scale) {
	return Vector2(x*scale, y*scale);
}

Vector2 Vector2::operator*(Vector2& vec)
{
	return Vector2(x * vec.x, y * vec.y);
}

Vector2 Vector2::operator/(float scale)
{
	return { x / scale, y / scale };
}

bool Vector2::operator==(const Vector2& vec) const
{
	return (x == vec.x && y == vec.y);
}

bool Vector2::operator!=(const Vector2& vec) const
{
	return !(x == vec.x && y == vec.y);
}

bool Vector2::operator<=(const Vector2& vec) const
{
	return (x <= vec.x && y <= vec.y);
}

bool Vector2::operator<(const Vector2& vec) const
{
	return (x < vec.x&& y < vec.y);
}

bool Vector2::operator>=(const Vector2& vec) const
{
	return (x >= vec.x && y >= vec.y);
}

bool Vector2::operator>(const Vector2& vec) const
{
	return  (x > vec.x && y > vec.y);
}

Vector2 operator+(const Vector2& va, const Vector2 vb) 
{
	Vector2 ret;
	ret.x = va.x + vb.x;
	ret.y = va.y + vb.y;
	return ret;
}

Vector2 operator-(const Vector2& va, const Vector2 vb) 
{
	return Vector2(va.x - vb.x, va.y - vb.y);
}

float Vector2::Magnitude()const
{
	return hypot(x, y);
}

void Vector2::Normalize() 
{
	float mag = Magnitude();
	x /= mag;
	y /= mag;
}

Vector2 Vector2::Normalized()
{
	float mag = Magnitude();
	return Vector2(x / mag, y / mag);
}

float Dot(const Vector2& va, const Vector2& vb)
{
	return va.x*vb.x + va.y*vb.y;
}

float Dot(const Vector3& va, const Vector3& vb)
{
	return va.x * vb.x +
		va.y * vb.y +
		va.z * vb.z;
}

float Cross(const Vector2& va, const Vector2& vb)
{
	return va.x * vb.y - vb.x * va.y;
}

Vector3 Cross(const Vector3& va, const Vector3& vb)
{
	Vector3 t;
	t.x = va.y * vb.z - va.z * vb.y;
	t.y = va.z * vb.x - va.x * vb.z;
	t.z = va.x * vb.y - va.y * vb.x;
	return t;
}

float Rad2DegF(float rad)
{
	return rad * (180.0f / DX_PI_F);
}

float Deg2RadF(float deg)
{
	return deg * (DX_PI_F / 180.0f);
}

double RadIn2PI(double rad)
{
	rad = fmod(rad, DX_TWO_PI);
	if (rad < 0.0)
	{
		rad += DX_TWO_PI;
	}
	return rad;
}

float SinCurve(float period, float count,float power)
{
	return sinf(DX_PI_F * 2.0f / period * count) * power;
}

Vector3 Lerp(const Vector3& start, const Vector3& end, float t)
{
	// 線形補間
	if (t >= 1.0f)
	{
		return end;
	}

	Vector3 ret = start;
	ret.x += t * (end.x - start.x);
	ret.y += t * (end.y - start.y);
	ret.z += t * (end.z - start.z);

	return ret;
}

Vector2Int Lerp(const Vector2Int& start, const Vector2Int& end, double t)
{
	// 線形補間
	if (t >= 1.0)
	{
		return end;
	}

	Vector2Int ret = start;
	ret.x += t * (end.x - start.x);
	ret.y += t * (end.y - start.y);

	return ret;
}

Vector3 ConvertVECTORtoVector3(const VECTOR& v3)
{
	Vector3 v;
	v.x = v3.x;
	v.y = v3.y;
	v.z = v3.z;
	return v;
}

//内積演算子
float operator*(const Vector2& va, const Vector2& vb)
{
	return Dot(va, vb);
}

//外積演算子
float operator%(const Vector2& va, const Vector2& vb) 
{
	return Cross(va, vb);
}

void Vector2::operator+=(const Vector2& v)
{
	Vector2(x += v.x, y += v.y);
}

void Vector2::operator+=(const float v)
{
	x += v;
	y += v;
}

void Vector2::operator-=(const Vector2& v) 
{
	Vector2(x -= v.x, y -= v.y);
}

void Vector2Int::operator+=(const Vector2Int& v)
{
	x += v.x;
	y += v.y;
}

Vector2Int Vector2Int::operator+(const Vector2Int& v)
{
	return Vector2Int(x + v.x,y + v.y);
}

Vector2Int Vector2Int::operator-(const Vector2Int& v)
{
	return Vector2Int(x - v.x, y - v.y);
}

void Vector2Int::operator-=(const Vector2Int& v)
{
	x -= v.x;
	y -= v.y;
}

void Vector2Int::operator*=(int scale)
{
	x *= scale;
	y *= scale;
}

Vector2Int Vector2Int::operator*(int scale)
{
	return Vector2Int(x * scale, y * scale);
}

bool Vector2Int::operator==(const Vector2Int& vec) const
{
	return (x == vec.x && y == vec.y);
}

bool Vector2Int::operator!=(const Vector2Int& vec) const
{
	return !(x == vec.x && y == vec.y);
}

bool Vector2Int::operator<=(const Vector2Int& vec) const
{
	return (x <= vec.x && y <= vec.y);
}

bool Vector2Int::operator<(const Vector2Int& vec) const
{
	return (x < vec.x&& y < vec.y);
}

bool Vector2Int::operator>=(const Vector2Int& vec) const
{
	return (x >= vec.x && y >= vec.y);
}

bool Vector2Int::operator>(const Vector2Int& vec) const
{
	return  (x > vec.x && y > vec.y);
}

Vector3 Line::vec()
{
	return end - p;
}

float Vector3::Magnitude() const
{
	return std::hypot(x, y, z);
}

void Vector3::Normalize()
{
	float mag = Magnitude();
	x /= mag;
	y /= mag;
	z /= mag;
}

Vector3 Vector3::Normalized()
{
	float mag = Magnitude();
	return Vector3(x / mag, y / mag, z / mag);
}

Vector3 Vector3::toDiopter()
{
	return Vector3(x * DX_PI_F / 180.0f, y * DX_PI_F / 180.0f, z * DX_PI_F / 180.0f);
}

VECTOR Vector3::toVECTOR()
{
	return VECTOR(x,y,z);
}

void Vector3::operator+=(const Vector3& v)
{
	Vector3(x += v.x, y += v.y, z += v.z);
}

void Vector3::operator-=(const Vector3& v)
{
	Vector3(x -= v.x, y -= v.y, z -= v.z);
}

void Vector3::operator*=(float scale)
{
	Vector3(x *= scale, y *= scale, z *= scale);
}

Vector3 Vector3::operator*(float scale)
{
	return Vector3(x * scale,y * scale,z * scale);
}

Vector3 Vector3::operator*(Vector3& vec)
{
	return Vector3(x * vec.x, y * vec.y, z * vec.z);
}

Vector3 Vector3::operator=(const Vector3& v)
{
	return Vector3(x = v.x, y = v.y, z = v.z);
}

Vector3 Vector3::operator/(float scale)
{
	return Vector3(x / scale, y / scale, z / scale);
}

Vector3 operator+(const Vector3& va, const Vector3 vb)
{
	return Vector3(va.x + vb.x, va.y + vb.y,va.z + vb.z);
}

Vector3 operator-(const Vector3& va, const Vector3 vb)
{
	return Vector3(va.x - vb.x, va.y - vb.y, va.z - vb.z);
}

bool Vector3::operator==(const Vector3& vec) const
{
	return (x == vec.x && y == vec.y && z == vec.z);
}

bool Vector3::operator!=(const Vector3& vec) const
{
	return !(x == vec.x && y == vec.y && z == vec.z);
}

bool Vector3::operator<=(const Vector3& vec) const
{
	return (x <= vec.x && y <= vec.y && z <= vec.z);
}

bool Vector3::operator<(const Vector3& vec) const
{
	return (x < vec.x && y < vec.y && z < vec.z);
}

bool Vector3::operator>=(const Vector3& vec) const
{
	return (x >= vec.x && y >= vec.y && z >= vec.z);
}

bool Vector3::operator>(const Vector3& vec) const
{
	return (x > vec.x && y > vec.y && z > vec.z);
}
