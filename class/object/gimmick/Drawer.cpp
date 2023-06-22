#include <DxLib.h>
#include "Drawer.h"
#include "../../scene/SceneManager.h"
#include "../../common/AnimationController.h"
#include "../../scene/SceneManager.h"
#include "../../common/OriginalShader.h"
#include "../../common/manager/SoundMng.h"
constexpr float animSpeed = 35.0f;

/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="���W"></param>
/// <param name="�p�x"></param>
/// <param name="�I�u�W�F�N�g��"></param>
/// <param name="������"></param>
Drawer::Drawer(std::string name, Vector3 pos, Vector3 angle, Vector3 scale, int num)
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

Drawer::~Drawer()
{
	Release();
}

//������
void Drawer::Init(void)
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
	origShader_ = std::make_unique<OriginalShader>(vsLight_, psLight_);

}

//�X�V
void Drawer::Update(void)
{
	//�C���^���N�g���ꂽ��
	if (interactFlg_)
	{
		//�Đ�����
		animFlg_ = true;
	}
	//�A�j���[�V��������
	Animation();
	interactFlg_ = false;
}

//�`��
void Drawer::Draw(void)
{
	//�`��
	origShader_->Draw(lpModelMng.GetID(modelStr_)[modelNum_]);
}

//�J��
void Drawer::Release(void)
{	
	//�V�F�[�_�[�폜
	DeleteShader(psLight_);
	DeleteShader(vsLight_);
}

int Drawer::GetModelNum(void)
{
	return modelNum_;
}

std::string Drawer::GetCollModelStr(void)
{
	return modelStr_;
}

void Drawer::VariableInit(void)
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
	//�A�j���[�V�������
	animState_ = GIMMICK_ANIM::CLOSE;
	//�M�~�b�N�^�C�v
	type_ = GIMMICK_TYPE::DRAWER;
	//���f���p�X
	modelStr_ = "resource/model/gimmick/drawer.mv1";
	//�A�j���[�V�����t���O
	animFlg_ = false;

	//�A�j���[�V�����ǉ�
	animController_ = std::make_unique<AnimationController>(lpModelMng.GetID(modelStr_)[modelNum_]);
	animController_->Add((int)GIMMICK_ANIM::OPEN, "resource/model/gimmick/anim/DrawerOpen.mv1", animSpeed);
	animController_->Add((int)GIMMICK_ANIM::CLOSE, "resource/model/gimmick/anim/DrawerClose.mv1", animSpeed);
}

void Drawer::Animation(void)
{

	//�Đ���
	if (animFlg_)
	{
		//���Ă���Ƃ�
		if (animState_ == GIMMICK_ANIM::CLOSE)
		{
			lpSoundMng.PlayingSound("resource/sound/DrawerOpenSE.mp3");
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
		else if (animState_ == GIMMICK_ANIM::OPEN)	//���Ă���Ƃ�
		{
			lpSoundMng.PlayingSound("resource/sound/DrawerCloseSE.mp3");
			//����
			animController_->Play((int)GIMMICK_ANIM::CLOSE, false);
			//�A�j���[�V��������~������
			if (animController_->GetPlayState() == ANIM_STATE::STOPPING)
			{
				//�J���Ă����Ԃɐ؂�ւ���
				animState_ = GIMMICK_ANIM::CLOSE;
				//�Đ��t���O�����~�߂�
				animFlg_ = false;
			}
		}
	}
	//�����蔻����X�V
	MV1RefreshCollInfo(lpModelMng.GetID(modelStr_)[modelNum_]);
	MV1SetPosition(lpModelMng.GetID(modelStr_)[modelNum_], pos_.toVECTOR());
	//�A�j���[�V�����X�V����
	animController_->Update(lpSceneMng.GetDeltaTime());
}
