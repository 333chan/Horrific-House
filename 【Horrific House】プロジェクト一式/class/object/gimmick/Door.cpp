#include "Door.h"
#include "../../common/AnimationController.h"
#include "../../Scene/SceneManager.h"
#include "../../common/OriginalShader.h"
#include "../../common/manager/SoundMng.h"
using namespace std;
constexpr float OPEN_SPEED = 18.0f;
constexpr float CLOSE_SPEED = 17.0f;


Door::Door(std::string name, Vector3 pos, Vector3 angle, Vector3 scale, int num)
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
	//������
	Init();
}

Door::~Door()
{
	Release();
}

void Door::Init(void)
{

	//�ϐ�������
	VariableInit();

	//���W��ݒ�
	MV1SetPosition(lpModelMng.GetID(modelStr_)[modelNum_],pos_.toVECTOR());
	
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

void Door::Update(void)
{
	//�C���^���N�g���ꂽ��
	if (interactFlg_)
	{
		//�A�j���[�V�����Đ�
		animFlg_ = true;
	}

	//�A�j���[�V��������
	Animation();
	interactFlg_ = false;
}

void Door::Draw(void)
{
	//�`��
	origShader_->Draw(lpModelMng.GetID(modelStr_)[modelNum_], hitFlg_, psLight_, vsLight_);
	//MV1DrawModelDebug(lpModelMng.GetID("./resource/model/gimmick/Door_White.mv1")[num_],0xffffff,1,0.001,1,1);
}

void Door::Release(void)
{
	//�V�F�[�_�[�폜
	DeleteShader(psLight_);
	DeleteShader(vsLight_);
}

int Door::GetModelNum(void)
{
	return modelNum_;
}

std::string Door::GetCollModelStr(void)
{
	return modelStr_;
}

void Door::Animation(void)
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
	//�����蔻����X�V
	MV1RefreshCollInfo(lpModelMng.GetID(modelStr_)[modelNum_]);
	//�A�j���[�V�����X�V����
	animController_->Update(lpSceneMng.GetDeltaTime());
}

void Door::VariableInit(void)
{
	//�C���^���N�g�t���O
	interactFlg_ = false;
	//�_�C�A���i���o�[
	dialNumber_ = 0;
	//���b�N���
	isLock_ = false;
	//�����蔻��t���O
	hitFlg_ = false;
	//�A�����b�N�i���o�[
	unlockNum_ = "0000";
	//�A�C�e��ID
	itemID_ = ItemID(0);
	//�����蔻��p�@��
	hitNormal_ = { 0.0f,0.0f,0.0f };
	//�A�j���[�V�����t���O
	animFlg_ = false;
	//�A�j���[�V�������
	animState_ = GIMMICK_ANIM::CLOSE;
	//�M�~�b�N�^�C�v
	type_ = GIMMICK_TYPE::DOOR;
	//�t�@�C���̃f�B���N�g��
	std::string directory = "resource/model/gimmick/";
	//�g�p���郂�f����ݒ�
	if (name_ == "Dark_A_Double")
	{
		modelStr_ = directory+"Door_Dark1_Double.mv1";

		//�A�j���[�V�����ǉ�
		animController_ = std::make_unique<AnimationController>(lpModelMng.GetID(modelStr_)[modelNum_]);
		animController_->Add((int)GIMMICK_ANIM::OPEN, "./resource/model/gimmick/anim/DoorDoubleOpen.mv1", OPEN_SPEED);
		animController_->Add((int)GIMMICK_ANIM::CLOSE, "./resource/model/gimmick/anim/DoorDoubleClose.mv1", CLOSE_SPEED);
	}
	else
	{
		modelStr_ = directory + "Door_Dark1.mv1";

		//�A�j���[�V�����ǉ�
		animController_ = std::make_unique<AnimationController>(lpModelMng.GetID(modelStr_)[modelNum_]);
		animController_->Add((int)GIMMICK_ANIM::OPEN, "./resource/model/gimmick/anim/DoorOpen.mv1", OPEN_SPEED);
		animController_->Add((int)GIMMICK_ANIM::CLOSE, "./resource/model/gimmick/anim/DoorClose.mv1", CLOSE_SPEED);
	}
}


