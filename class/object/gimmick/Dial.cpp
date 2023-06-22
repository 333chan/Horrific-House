#include "Dial.h"
#include "../../common/AnimationController.h"
#include "../../common/OriginalShader.h"
#include "../../common/manager/SoundMng.h"
constexpr float STOP_ANGLE = 36.0f;
constexpr float MAX_ANGLE = 360.0f;
constexpr float ROT_SPEED = 1.5f;

Dial::Dial(std::string name, Vector3 pos, Vector3 angle, Vector3 scale, int num)
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

Dial::~Dial()
{
	Release();
}

void Dial::Init(void)
{
	//�ϐ�������
	VariableInit();

	//�ʒu�ݒ�
	MV1SetPosition(lpModelMng.GetID(modelStr_)[modelNum_],pos_.toVECTOR());

	//�g��l�ݒ�
	MV1SetScale(lpModelMng.GetID(modelStr_)[modelNum_], VECTOR(scale_.toVECTOR()));

	//�x�����烉�W�A���ɕϊ�
	angle_ = { Deg2RadF(angle_.x),Deg2RadF(angle_.y),Deg2RadF(angle_.z) };

	//��]
	MV1SetRotationXYZ(lpModelMng.GetID(modelStr_)[modelNum_], angle_.toVECTOR());

	//���C�e�B���O
	vsLight_ = LoadVertexShader("./resource/shader/vs/NormalModel_vs.vso");
	psLight_ = LoadPixelShader("./resource/shader/ps/GameModel_ps.pso");

	//�V�F�[�_�[�ݒ�
	origShader_ = std::make_unique<OriginalShader>(vsLight_, psLight_);
}

void Dial::Update(void)
{
	//�C���^���N�g���ꂽ��
	if (interactFlg_)
	{
		//�A�j���[�V�����Đ�
		animFlg_ = true;
	}

	//��]
	Rotaion();
}

//�`��
void Dial::Draw(void)
{
	origShader_->Draw(lpModelMng.GetID(modelStr_)[modelNum_]);
}

//�J��
void Dial::Release(void)
{
	//�V�F�[�_�[�폜
	DeleteShader(psLight_);
	DeleteShader(vsLight_);
}
//���f���ԍ���Ԃ�
int Dial::GetModelNum(void)
{
	return modelNum_;
}
//���f�����������Ԃ�
std::string Dial::GetCollModelStr(void)
{
	return modelStr_;
}

void Dial::VariableInit(void)
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
	animState_ = GIMMICK_ANIM::NONE;

	//�M�~�b�N�^�C�v
	type_ = GIMMICK_TYPE::DIAL;

	//��~�p�x
	stopAngle_ = 0.0f;

	//���f���p�X
	modelStr_ = "resource/model/gimmick/Dial.mv1";
}

void Dial::Rotaion(void)
{
	//��~��
	if (!animFlg_)
	{
		//��~�ʒu�����߂�
		stopAngle_ = angle_.x + Deg2RadF(STOP_ANGLE);

		//angle_��360�x�ɂȂ�����
		if (angle_.x >= Deg2RadF(MAX_ANGLE))
		{
			//angle_�̒l�����������Ȃ��悤0�ɖ߂�
			angle_.x = 0.0f;
		}
		//���݂̃_�C�����ԍ�(���݂̊p�x/��~�ʒu)
		dialNumber_ = static_cast<int>(angle_.x / Deg2RadF(STOP_ANGLE));
	}
	else	//�i�s��
	{
		lpSoundMng.PlaySoundOneTime("resource/sound/DialSE.mp3");
		//angle_����~�p�x��菬�����Ȃ�
		if(angle_.x <= stopAngle_)
		{
			//��]
			angle_.x += Deg2RadF(ROT_SPEED);
		}
		else	//��~�p�x�𒴂�����
		{
			lpSoundMng.ResetCnt("resource/sound/DialSE.mp3");
			//��~����
			animFlg_ = false;
		}
		interactFlg_ = false;
	}
	//��]�X�V
	MV1SetRotationXYZ(lpModelMng.GetID(modelStr_)[modelNum_], angle_.toVECTOR());
}
