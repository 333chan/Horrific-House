#pragma once
#include <DxLib.h>
#include <string>
#include "Quaternion.h"
#include "Geometry.h"

struct Transform
{
	// モデルのハンドルID
	int modelHandle_ = -1;

	// 大きさ
	Vector3 scale_;
	// 回転
	Vector3 rot_;
	// 位置
	Vector3 pos_;

	MATRIX matScale_ = {};
	MATRIX matRot_ = {};
	MATRIX matPos_ = {};

	// 回転
	Quaternion quaRot_;

	// ローカル回転
	Quaternion quaRotLocal_;

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="modelName">モデルの名前</param>
	/// <param name="pos">位置</param>
	/// <param name="scale">大きさ</param>
	/// <param name="quaRot">回転</param>
	/// <param name="quaRotLocal">ローカル回転</param>
	void Init(std::string modelName, Vector3 pos, Vector3 scale, Vector3 quaRot, Vector3 quaRotLocal);

	// 更新
	void Update(void);

	//描画
	void Draw(void);

	//破棄
	void Release(void);

	// 前方方向を取得
	VECTOR GetForward(void);

	// 後方方向を取得
	VECTOR GetBack(void);

	// 右方向を取得
	VECTOR GetRight(void);

	// 左方向を取得
	VECTOR GetLeft(void);

	// 上方向を取得
	VECTOR GetUp(void);

	// 下方向を取得
	VECTOR GetDown(void);

	// 対象方向を取得
	VECTOR GetDir(VECTOR vec);
};

