#pragma once
#include<DxLib.h>

//サイズを表す構造体
struct Size {
	float w;//幅
	float h;//高さ
};

//座標を表す構造体
struct Vector2 {
	Vector2():x(0),y(0){}
	Vector2(float inx,float iny):x(inx),y(iny){}
	float x, y;
	///ベクトルの大きさを返します
	float Magnitude()const;
	
	///正規化(大きさを１に)します
	void Normalize();

	///正規化ベクトルを返します
	Vector2 Normalized();

	void operator+=(const Vector2& v);
	void operator+=(const float v);

	void operator-=(const Vector2& v);
	void operator*=(float scale);
	Vector2 operator*(float scale);
	Vector2 operator*(Vector2& vec);

	Vector2 operator/(float scale);
	Vector2 operator-() {
		return Vector2(-x, -y);
	}

	//比較演算子
	bool operator ==(const Vector2& vec)const;
	bool operator !=(const Vector2& vec)const;
	bool operator <=(const Vector2& vec)const;
	bool operator <(const Vector2& vec)const;
	bool operator >=(const Vector2& vec)const;
	bool operator >(const Vector2& vec)const;

};

struct Vector2Int {
	Vector2Int():x(0),y(0){}
	Vector2Int(int inx,int iny):x(inx),y(iny){}
	int x, y;

	void operator+=(const Vector2Int& v);
	void operator-=(const Vector2Int& v);
	void operator*=(int scale);
	Vector2Int operator+(const Vector2Int& v);
	Vector2Int operator-(const Vector2Int& v);
	Vector2Int operator*(int scale);
	Vector2Int operator-() {return Vector2Int(-x, -y);}
	Vector2Int operator+() {return Vector2Int(+x, +y);}

	//比較演算子
	bool operator ==(const Vector2Int& vec)const;
	bool operator !=(const Vector2Int& vec)const;
	bool operator <=(const Vector2Int& vec)const;
	bool operator <(const Vector2Int& vec)const;
	bool operator >=(const Vector2Int& vec)const;
	bool operator >(const Vector2Int& vec)const;
};

Vector2 operator+(const Vector2& va, const Vector2 vb);
Vector2 operator-(const Vector2& va, const Vector2 vb);

struct Vector3 {
	Vector3() :x(0.0f), y(0.0f), z(0.0f) {}
	Vector3(float inx, float iny,float inz) :x(inx), y(iny) ,z(inz) {}
	float x, y, z;

	///ベクトルの大きさを返します
	float Magnitude()const;
	
	///正規化(大きさを１に)します
	void Normalize();

	///正規化ベクトルを返します
	Vector3 Normalized();
	Vector3 toDiopter();

	//VECTORに変換
	VECTOR toVECTOR();

	void operator+=(const Vector3& v);
	void operator-=(const Vector3& v);
	void operator*=(float scale);
	Vector3 operator*(float scale);
	Vector3 operator*(Vector3& vec);
	Vector3 operator=(const Vector3& v);

	Vector3 operator/(float scale);
	Vector3 operator-() {
		return Vector3(-x, -y,-z);
	}

	//比較演算子
	bool operator ==(const Vector3& vec)const;
	bool operator !=(const Vector3& vec)const;
	bool operator <=(const Vector3& vec)const;
	bool operator <(const Vector3& vec)const;
	bool operator >=(const Vector3& vec)const;
	bool operator >(const Vector3& vec)const;

};

Vector3 operator+(const Vector3& va, const Vector3 vb);
Vector3 operator-(const Vector3& va, const Vector3 vb);

struct Line
{
	Vector3 p;		//始点
	Vector3 end;	//終点
	Vector3 vec();
};

//内積を返す
float Dot(const Vector2& va, const Vector2& vb);
float Dot(const Vector3& va, const Vector3& vb);

//外積を返す
float Cross(const Vector2& va, const Vector2& vb);
Vector3 Cross(const Vector3& va, const Vector3& vb);

// ラジアン(rad)から度(deg)に変換
float Rad2DegF(float rad);

// 度(deg)からラジアン(rad)に変換
float Deg2RadF(float deg);

// 0(0)～2π(360度)の範囲に収める
double RadIn2PI(double rad);

//サインカーブ
float SinCurve(float period, float count,float power);

// 線形補間
Vector3 Lerp(const Vector3& start,const Vector3& end, float t);
Vector2Int Lerp(const Vector2Int& start,const Vector2Int& end, double t);

//Vector3に変換
Vector3 ConvertVECTORtoVector3(const VECTOR& v3);

///内積演算子
float operator*(const Vector2& va, const Vector2& vb);

///外積演算子
float operator%(const Vector2& va, const Vector2& vb);
