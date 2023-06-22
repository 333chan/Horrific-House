#pragma once
#include "Geometry.h"
// ディレクションライト構造体
struct DirectionlLight
{
	Vector3  color;     // ライトのカラー
	float    pad0;

	Vector3  direction; // ライトの方向
	float   pad1;

};

//ポイントライト構造体
struct PointLight
{
	Vector3 position;   // 座標
	float   pad0;

	Vector3 color;      // ライトのカラー
	float   range;      // ライトの影響を与える範囲

};

// スポットライト構造体
struct SpotLight
{
	Vector3 Position;     // 位置
	float   pad0;

	Vector3 Color;        // カラー
	float   Range;        // 影響範囲

	Vector3 Direction;    // 射出方向
	float   Angle;        // 射出角度
};