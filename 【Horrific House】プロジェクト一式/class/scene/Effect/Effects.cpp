#include "Effects.h"

void Effects::SetEffect(const VECTOR& pos, const VECTOR& nor, float scale,float speed,bool isLoop)
{
	flag_ = true;
	pos_ = pos;
	nor_ = nor;
	scale_ = scale;
	isLoop_ = isLoop;
	speed_ = speed;
}

void Effects::PlayEffect(const EffekseerEffectHandle& handle)
{
	if (!isLoop_)
	{
		if (flag_)
		{
			//再生チェック
			if (handle_.IsPlaying())
			{
				handle_.Stop();
			}
			//再生
			handle_ = handle.Play3D();
			//場所指定
			handle_.SetPos(pos_);
			//向き指定
			handle_.SetRotation(atan2(nor_.y, std::hypotf(nor_.x, nor_.z)), atan2(-nor_.x, -nor_.z), 0);
			//大きさ指定
			handle_.SetScale(scale_);
			handle_.SetSpeed(speed_);
			//開始flagを切る
			flag_ = false;
		}
	}
	else
	{
		if (flag_)
		{
			//再生チェック
			if (handle_.IsPlaying())
			{
				handle_.Stop();
			}
			//再生
			handle_ = handle.Play3D();
			//場所指定
			handle_.SetPos(pos_);
			//向き指定
			handle_.SetRotation(atan2(nor_.y, std::hypotf(nor_.x, nor_.z)), atan2(-nor_.x, -nor_.z), 0);
			//大きさ指定
			handle_.SetScale(scale_);
			handle_.SetSpeed(speed_);
			//開始flagを切る
			flag_ = false;
		}
		//再生チェック
		if (!handle_.IsPlaying())
		{
			flag_ = true;
		}
	}
}

void Effects::StopEffect(const EffekseerEffectHandle& handle)
{
	//再生チェック
	if (handle_.IsPlaying())
	{
		handle_.Stop();
	}
}

void Effects::SetPos(VECTOR pos)
{
	pos_ = pos;
}


