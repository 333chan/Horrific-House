#include "DialFrame.h"
#include "../../common/OriginalShader.h"

DialFrame::DialFrame(std::string name, Vector3 pos, Vector3 angle, Vector3 scale, int num)
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

DialFrame::~DialFrame()
{
}

void DialFrame::Init(void)
{
	//�ϐ�������
	VariableInit();

	//�ʒu�ݒ�
	MV1SetPosition(lpModelMng.GetID(modelStr_)[modelNum_], pos_.toVECTOR());
	
	//�g��l�ݒ�
	MV1SetScale(lpModelMng.GetID(modelStr_)[modelNum_], VECTOR(scale_.toVECTOR()));
	
	//�x�����烉�W�A���ɕϊ�
	angle_ = { Deg2RadF(angle_.x),Deg2RadF(angle_.y),Deg2RadF(angle_.z) };
	
	//��]
	MV1SetRotationXYZ(lpModelMng.GetID(modelStr_)[modelNum_], angle_.toVECTOR());
	
	//���C�e�B���O
	int VertexShader = LoadVertexShader("resource/shader/vs/NormalModel_vs.vso");
	int PixelShader = LoadPixelShader("resource/shader/ps/GameModel_ps.pso");
	origShader_ = std::make_unique<OriginalShader>(VertexShader, PixelShader);
}

void DialFrame::Update(void)
{
}

void DialFrame::Draw(void)
{
	origShader_->Draw(lpModelMng.GetID(modelStr_)[modelNum_]);
}

void DialFrame::Release(void)
{
}

int DialFrame::GetModelNum(void)
{
	return modelNum_;
}

std::string DialFrame::GetCollModelStr(void)
{
    return modelStr_;
}

void DialFrame::VariableInit(void)
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
	type_ = GIMMICK_TYPE::FRAME;
	//���f���p�X
	modelStr_ = "resource/model/gimmick/Dial_Frame.mv1";
}