#include "DialLockDoor.h"
#include "../../common/AnimationController.h"
#include "../../scene/SceneManager.h"
#include "../../common/OriginalShader.h"
#include "../../common/manager/SoundMng.h"
constexpr double UI_DRAW_TIME = 1.3;		//UI�\������
constexpr int OFFSET_Y = 370;		//�I�t�Z�b�g
constexpr float OPEN_SPEED = 18.0f;
constexpr float CLOSE_SPEED = 17.0f;

DialLockDoor::DialLockDoor(std::string name, Vector3 pos, Vector3 angle, Vector3 scale, int num,std::string unlockNum)
{
	//�I�u�W�F�N�g��
	name_ = name;
	//���W
	pos_ = pos;
	//�p�x
	angle_ = angle;
	//�g��l
	scale_ = scale;
	//���f��������
	modelNum_ = num;
	//���b�N�����̂��߂̔ԍ�
	unlockNum_	= unlockNum;
	//������
	Init();
}

DialLockDoor::~DialLockDoor()
{
	Release();
}

void DialLockDoor::Init(void)
{
	//�ϐ�������
	VariableInit();

	//���W��ݒ�
	MV1SetPosition(lpModelMng.GetID(modelStr_)[modelNum_], pos_.toVECTOR());

	//�g��l��ݒ�
	MV1SetScale(lpModelMng.GetID(modelStr_)[modelNum_], VECTOR(scale_.toVECTOR()));

	//�x�����烉�W�A���ɕϊ�
	angle_ = { Deg2RadF(angle_.x),Deg2RadF(angle_.y),Deg2RadF(angle_.z) };
	//��]
	MV1SetRotationXYZ(lpModelMng.GetID(modelStr_)[modelNum_], angle_.toVECTOR());

	//���C�e�B���O
	vsLight_ = LoadVertexShader("./resource/shader/vs/NormalModel_vs.vso");
	psLight_ = LoadPixelShader("./resource/shader/ps/GameModel_ps.pso");
	//�A�E�g���C��
	int outL_vs = LoadVertexShader("./resource/shader/vs/OutLine_vs.vso");
	int outL_ps = LoadPixelShader("./resource/shader/ps/OutLine_ps.pso");
	origShader_ = std::make_unique<OriginalShader>(outL_vs, outL_ps);
}

void DialLockDoor::Update(void)
{
	//�C���^���N�g���ꂽ��
	if (interactFlg_)
	{
		//���b�N��
		if (isLock_)
		{
			//�K�`���K�`�������Đ�
			lpSoundMng.PlayingSound("resource/sound/LockDoorSE.mp3");
		}
		else	//���b�N��������Ă�����
		{
			//�A�j���[�V�����Đ�
			animFlg_ = true;
		}
		//UI�\��
		uiFlg_ = true;
	}
	//�A�j���[�V��������
	Animation();

	//UI�\�����Ԍv�Z
	DrawTimeCount();

	interactFlg_ = false;
}

void DialLockDoor::Draw(void)
{
	//�`��
	origShader_->Draw(lpModelMng.GetID(modelStr_)[modelNum_], hitFlg_, psLight_, vsLight_);
	DrawUI();
}

void DialLockDoor::Release(void)
{
	//�V�F�[�_�[�폜
	DeleteShader(psLight_);
	DeleteShader(vsLight_);
}

int DialLockDoor::GetModelNum(void)
{
	return modelNum_;
}

std::string DialLockDoor::GetCollModelStr(void)
{
	return modelStr_;
}

void DialLockDoor::VariableInit(void)
{
	//�C���^���N�g�t���O
	interactFlg_ = false;

	//�_�C�A���i���o�[
	dialNumber_ = 0;

	//���b�N���
	isLock_ = true;

	//�����蔻��t���O
	hitFlg_ = false;

	//�A�C�e��ID
	itemID_ = ItemID(0);

	//�����蔻��p�@��
	hitNormal_ = { 0.0f,0.0f,0.0f };

	//�o�ߎ���
	delta_ = 0.0;

	//UI�\���t���O
	uiFlg_ = false;

	//���񔻒�p�t���O
	firstTimeFlg_ = true;

	//�A�j���[�V�������
	animState_ = GIMMICK_ANIM::CLOSE;

	//�M�~�b�N�^�C�v
	type_ = GIMMICK_TYPE::DL_DOOR;

	//�摜�T�C�Y�̔����̒l
	graphSize_ = { 0,0 };

	//�`��ʒu
	drawPos = { lpSceneMng.GetScreenSize().x / 2 ,(lpSceneMng.GetScreenSize().y / 2) + OFFSET_Y };
	
	//���f���p�X
	modelStr_ = "resource/model/gimmick/Door_Red2.mv1";

	//�A�j���[�V�����ǉ�
	animController_ = std::make_unique<AnimationController>(lpModelMng.GetID(modelStr_)[modelNum_]);
	animController_->Add((int)GIMMICK_ANIM::OPEN, "resource/model/gimmick/anim/DoorOpen.mv1", OPEN_SPEED);
	animController_->Add((int)GIMMICK_ANIM::CLOSE, "resource/model/gimmick/anim/DoorClose.mv1", CLOSE_SPEED);
}

void DialLockDoor::Animation(void)
{
	//�A�j���[�V�����̏�Ԃ��󂯎��
	ANIM_STATE animState = animController_->GetPlayState();

	//���b�N������
	if (!isLock_)
	{
		//�Đ���
		if (animFlg_)
		{
			//���Ă���Ƃ�
			if (animState_ == GIMMICK_ANIM::CLOSE)
			{
				lpSoundMng.PlayingSound("resource/sound/DoorSE.mp3");
				//�J����
				animController_->Play((int)GIMMICK_ANIM::OPEN, false);
				//�A�j���[�V��������~������
				if (animController_->GetPlayState() == ANIM_STATE::STOPPING)
				{
					//�J���Ă����Ԃɐ؂�ւ���
					animState_ = GIMMICK_ANIM::OPEN;
					//�Đ��t���O�����~�߂�
					animFlg_ = false;
				}
			}
			else if (animState_ == GIMMICK_ANIM::OPEN)	//�J���Ă���Ƃ�
			{
				//
				lpSoundMng.PlayingSound("resource/sound/DoorSE.mp3");
				//����
				animController_->Play((int)GIMMICK_ANIM::CLOSE, false);
				//�A�j���[�V��������~������
				if (animController_->GetPlayState() == ANIM_STATE::STOPPING)
				{
					//���Ă����Ԃɐ؂�ւ���
					animState_ = GIMMICK_ANIM::CLOSE;
					//�Đ��t���O�����~�߂�
					animFlg_ = false;
					lpSoundMng.PlayingSound("resource/sound/DoorCloseSE.mp3");
				}
			}
		}
	}
	//�����蔻����X�V
	MV1RefreshCollInfo(lpModelMng.GetID(modelStr_)[modelNum_]);
	//�A�j���[�V�����X�V����
	animController_->Update(lpSceneMng.GetDeltaTime());
}

void DialLockDoor::DrawTimeCount(void)
{
	//�\����
	if (uiFlg_)
	{
		delta_ += lpSceneMng.GetDeltaTime();
	}
	else	//��\����
	{
		//���Ԃ�������
		delta_ = 0.0;
	}

	//�o�ߎ��Ԃ��w�肵�����Ԃ𒴂�����
	if (delta_ >= UI_DRAW_TIME)
	{
		//UI������
		uiFlg_ = false;
		//���b�N����������Ă�����
		if (!isLock_)
		{
			//����UI��1�񂾂��\�����邽�߃t���O��false��
			firstTimeFlg_ = false;
		}
	}
}

void DialLockDoor::DrawUI(void)
{
	GetGraphSize(lpImageMng.GetID("DialLockUI")[0], &graphSize_.x, &graphSize_.y);
	graphSize_.x /= 2;
	graphSize_.y /= 2;
	if (uiFlg_ && firstTimeFlg_)
	{
		if (isLock_)
		{
			DrawGraph(drawPos.x - graphSize_.x, drawPos.y, lpImageMng.GetID("DialLockUI")[0], true);
		}
	}
}
