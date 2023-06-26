#include "JukeBox.h"
#include "../../common/manager/SoundMng.h"
#include "../../common/OriginalShader.h"

JukeBox::JukeBox(std::string name, Vector3 pos, Vector3 angle, Vector3 scale, int num)
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

JukeBox::~JukeBox()
{
	Release();
}

void JukeBox::Init(void)
{
	//�ϐ�������
	VariableInit();

	//���W��ݒ�
	MV1SetPosition(lpModelMng.GetID(modelStr_)[modelNum_], pos_.toVECTOR());

	//�g��l��ݒ�
	MV1SetScale(lpModelMng.GetID(modelStr_)[modelNum_], scale_.toVECTOR());

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
	//lpSoundMng.Play3DSound("./resource/sound/test.wav", pos_, 6500.0f);
}

void JukeBox::Update(void)
{
	//Set3DPositionSoundMem(pos_.toVECTOR(), lpSoundMng.GetID("./resource/sound/test.wav")[0]);
	
	static std::string soundFileName = "./resource/sound/jukeBoxBGM.mp3";
	
	if (interactFlg_)
	{
		//���y������Ă��邩�`�F�b�N
		if (CheckSoundMem(lpSoundMng.GetID(soundFileName)[0]) == 1)
		{
			soundFlg_ = true;
		}
		else
		{
			soundFlg_ = false;
		}
	}

	// ����Ă��鎞
	if (soundFlg_)
	{
		// �����~�߂�
		lpSoundMng.StopSound(soundFileName);
	}

	// �~�܂��Ă��鎞
	if (!soundFlg_)
	{
		// ���𗬂�
		lpSoundMng.Play3DSound(soundFileName, pos_, 15000.0f);
	}

	// �K��interactFlg_��false��
	interactFlg_ = false;
}

void JukeBox::Draw(void)
{
	origShader_->Draw(lpModelMng.GetID(modelStr_)[modelNum_], hitFlg_, psLight_, vsLight_);
}

void JukeBox::Release(void)
{
	//�V�F�[�_�[�폜
	DeleteShader(psLight_);
	DeleteShader(vsLight_);
}

int JukeBox::GetModelNum(void)
{
	return modelNum_;
}

std::string JukeBox::GetCollModelStr(void)
{
	return modelStr_;
}

void JukeBox::VariableInit(void)
{
	//�C���^���N�g�t���O
	interactFlg_ = false;
	//�T�E���h�Đ��t���O
	soundFlg_ = true;
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
	animState_ = GIMMICK_ANIM::NONE;
	//�M�~�b�N�^�C�v
	type_ = GIMMICK_TYPE::JUKE_BOX;
	//���f���p�X
	modelStr_ = "resource/model/gimmick/JukeBox.mv1";
}
