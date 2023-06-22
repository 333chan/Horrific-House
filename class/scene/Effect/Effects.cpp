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
			//�Đ��`�F�b�N
			if (handle_.IsPlaying())
			{
				handle_.Stop();
			}
			//�Đ�
			handle_ = handle.Play3D();
			//�ꏊ�w��
			handle_.SetPos(pos_);
			//�����w��
			handle_.SetRotation(atan2(nor_.y, std::hypotf(nor_.x, nor_.z)), atan2(-nor_.x, -nor_.z), 0);
			//�傫���w��
			handle_.SetScale(scale_);
			handle_.SetSpeed(speed_);
			//�J�nflag��؂�
			flag_ = false;
		}
	}
	else
	{
		if (flag_)
		{
			//�Đ��`�F�b�N
			if (handle_.IsPlaying())
			{
				handle_.Stop();
			}
			//�Đ�
			handle_ = handle.Play3D();
			//�ꏊ�w��
			handle_.SetPos(pos_);
			//�����w��
			handle_.SetRotation(atan2(nor_.y, std::hypotf(nor_.x, nor_.z)), atan2(-nor_.x, -nor_.z), 0);
			//�傫���w��
			handle_.SetScale(scale_);
			handle_.SetSpeed(speed_);
			//�J�nflag��؂�
			flag_ = false;
		}
		//�Đ��`�F�b�N
		if (!handle_.IsPlaying())
		{
			flag_ = true;
		}
	}
}

void Effects::StopEffect(const EffekseerEffectHandle& handle)
{
	//�Đ��`�F�b�N
	if (handle_.IsPlaying())
	{
		handle_.Stop();
	}
}

void Effects::SetPos(VECTOR pos)
{
	pos_ = pos;
}


