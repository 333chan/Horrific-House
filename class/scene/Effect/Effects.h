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

	void SetEffect(const VECTOR& pos, const VECTOR& nor, float scale, float speed, bool isLoop);//Effect�̏ꏊ���w�肵�A�J�nflag�𗧂Ă�

	void PlayEffect(const EffekseerEffectHandle& handle);

	void StopEffect(const EffekseerEffectHandle& handle);

	void SetPos(VECTOR pos);
private:
	bool flag_ = false;					//Effect�̕\��flag
	size_t id_ = 0;						//ID
	Effekseer3DPlayingHandle handle_;	//�n���h��
	VECTOR pos_;						//�ꏊ
	VECTOR nor_;						//Y���̌���
	float scale_ = 1.0f;					//�X�P�[��
	float speed_;
	bool isLoop_ = false;

	

};

