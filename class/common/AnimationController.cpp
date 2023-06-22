#include <DxLib.h>
#include "AnimationController.h"

AnimationController::AnimationController(int modelId)
{
	//���f��ID
	model_ = modelId;
	//�A�j���[�V�����̎��
	animType_ = -1;
	//���[�v���邩
	mIsLoop = false;
	//��~���Ă��邩
	mIsStop = false;
	//�Đ����
	playType = PLAY_TYPE::NORMAL;
	//�A�j���[�V�������
	animState_ = ANIM_STATE::STOPPING;
}

AnimationController::~AnimationController()
{
	//�A�j���[�V�����폜
	playAnim_.attachNo = MV1DetachAnim(model_, playAnim_.attachNo);
}

void AnimationController::Add(int type, std::string path, float speed)
{
	Animation anim;
	//���f�����[�h
	anim.model = MV1LoadModel(path.c_str());
	//�A�j���[�V�����ԍ��ݒ�
	anim.attachNo = type;
	//�Đ����x
	anim.speed = speed;

	if (animations_.count(type) == 0)
	{
		// �ǉ�
		animations_.emplace(type, anim);
	}
	else
	{
		// ����ւ�
		animations_[type].model = anim.model;
		animations_[type].attachNo = anim.attachNo;
		animations_[type].totalTime = anim.totalTime;
	}
}

void AnimationController::Add(int type, int handle, float speed)
{
	Animation anim;
	//�n���h��
	anim.model = handle;
	//�A�j���[�V�������
	anim.attachNo = type;
	//�Đ����x
	anim.speed = speed;

	if (animations_.count(type) == 0)
	{
		// �ǉ�
		animations_.emplace(type, anim);
	}
	else
	{
		// ����ւ�
		animations_[type].model = anim.model;
		animations_[type].attachNo = anim.attachNo;
		animations_[type].totalTime = anim.totalTime;
	}
}

void AnimationController::Play(int type, bool isLoop)
{
	if (animType_ != type) {

		if (animType_ != -1)
		{
			// ���f������A�j���[�V�������O��
			playAnim_.attachNo = MV1DetachAnim(model_, playAnim_.attachNo);
		}

		// �A�j���[�V������ʂ�ύX
		animType_ = type;
		playAnim_ = animations_[type];

		// ������
		playAnim_.frame = 0.0f;

		// ���f���ɃA�j���[�V������t����
		playAnim_.attachNo = MV1AttachAnim(model_, 0, playAnim_.model);

		// �A�j���[�V���������Ԃ̎擾
		playAnim_.totalTime = MV1GetAttachAnimTotalTime(model_, playAnim_.attachNo);
		
		//�Đ����@
		playType = PLAY_TYPE::NORMAL;
		animState_ = ANIM_STATE::PLAYING;

		// �A�j���[�V�������[�v
		mIsLoop = isLoop;
	}
}

void AnimationController::Play(int type, int num, bool isLoop)
{
	if (animType_ != type) {

		if (animType_ != -1)
		{
			// ���f������A�j���[�V�������O��
			playAnim_.attachNo = MV1DetachAnim(model_, playAnim_.attachNo);
		}

		// �A�j���[�V������ʂ�ύX
		animType_ = type;
		playAnim_ = animations_[type];

		// ������
		playAnim_.frame = 0.0f;

		// ���f���ɃA�j���[�V������t����
		playAnim_.attachNo = MV1AttachAnim(model_, num, playAnim_.model);

		// �A�j���[�V���������Ԃ̎擾
		playAnim_.totalTime = MV1GetAttachAnimTotalTime(model_, playAnim_.attachNo);

		//�Đ����@
		playType = PLAY_TYPE::NORMAL;
		animState_ = ANIM_STATE::PLAYING;

		// �A�j���[�V�������[�v
		mIsLoop = isLoop;
	}
}

//�t�Đ�
void AnimationController::PlayBack(int type, bool isLoop)
{
	if (animType_ != type) {

		if (animType_ != -1)
		{
			// ���f������A�j���[�V�������O��
			playAnim_.attachNo = MV1DetachAnim(model_, playAnim_.attachNo);
		}

		// �A�j���[�V������ʂ�ύX
		animType_ = type;
		playAnim_ = animations_[type];

		// ���f���ɃA�j���[�V������t����
		playAnim_.attachNo = MV1AttachAnim(model_, type, playAnim_.model);

		// �A�j���[�V���������Ԃ̎擾
		playAnim_.totalTime = MV1GetAttachAnimTotalTime(model_, playAnim_.attachNo);
		
		// ������
		//���t�Đ��Ȃ̂Ńt���[�������ő�l��
		playAnim_.frame = playAnim_.totalTime;


		//�Đ����@
		playType = PLAY_TYPE::REVERSE;

		// �A�j���[�V�������[�v
		mIsLoop = isLoop;
	}
}

void AnimationController::Update(double delta)
{
	// �A�j���[�V�����I������
	bool isEnd = false;

	if (!mIsStop)
	{
		// �ʏ�Đ��̏ꍇ
		if (playType == PLAY_TYPE::NORMAL)
		{
			playAnim_.frame += (delta * playAnim_.speed);

			//�t���[���������Đ����Ԃ𒴂�����
			if (playAnim_.frame > playAnim_.totalTime)
			{
				//�I��
				isEnd = true;
			}
			if (isEnd)
			{
				// ���[�v������
				if (mIsLoop)
				{
					// �t���[�������ŏ��ɖ߂�
					playAnim_.frame = 0.0f;
				}
				else	// ���[�v���Ȃ�
				{
					//��~
					animState_ = ANIM_STATE::STOPPING;
					//�t���[�������ő�ŌŒ�
					playAnim_.frame = playAnim_.totalTime;
				}
			}

		}
	}

	// �A�j���[�V�����ݒ�
	MV1SetAttachAnimTime(model_, playAnim_.attachNo, playAnim_.frame);
}


void AnimationController::Release(void)
{
	//���f���폜
	for (auto& anim : animations_)
	{
		MV1DeleteModel(anim.second.model);
	}
}


ANIM_STATE AnimationController::GetPlayState(void)
{
	return animState_;
}


