#pragma once
#include "../../common/EffekseerEffectHandle.hpp"
#include <memory>
#include <array>
#include <vector>
#include <cmath>
#include"../../common/Geometry.h"

class Effects
{
public:

	void SetEffect(const VECTOR& pos, const VECTOR& nor, float scale, float speed, bool isLoop);//Effectの場所を指定し、開始flagを立てる

	void PlayEffect(const EffekseerEffectHandle& handle);

	void StopEffect(const EffekseerEffectHandle& handle);

	void SetPos(VECTOR pos);
private:
	bool flag_ = false;					//Effectの表示flag
	size_t id_ = 0;						//ID
	Effekseer3DPlayingHandle handle_;	//ハンドル
	VECTOR pos_;						//場所
	VECTOR nor_;						//Y軸の向き
	float scale_ = 1.0f;					//スケール
	float speed_;
	bool isLoop_ = false;

	

};

