#include "Water.h"
#include "../../common/OriginalShader.h"

constexpr float DIV_VALUE = 1000.0f;

Water::Water(std::string name, Vector3 pos, Vector3 angle, Vector3 scale, int num)
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

Water::~Water()
{
}

void Water::Init(void)
{
	//�ϐ�������
	VariableInit();
	//���W��ݒ�
	MV1SetPosition(lpModelMng.GetID(modelStr_)[modelNum_], pos_.toVECTOR());

	//�g��l��ݒ�
	scale_ = { scale_.x / DIV_VALUE,scale_.y / DIV_VALUE,scale_.z / DIV_VALUE };
	MV1SetScale(lpModelMng.GetID(modelStr_)[modelNum_], scale_.toVECTOR());

	//�x�����烉�W�A���ɕϊ�
	angle_ = { Deg2RadF(angle_.x),Deg2RadF(angle_.y),Deg2RadF(angle_.z) };
	//��]
	MV1SetRotationXYZ(lpModelMng.GetID(modelStr_)[modelNum_], angle_.toVECTOR());
	//��
	int vs = LoadVertexShader("./resource/shader/vs/NormalModel_vs.vso");
	int ps = LoadPixelShader("./resource/shader/ps/Water_ps.pso");

	origShader_ = std::make_unique<OriginalShader>(vs, ps);
}

void Water::Update(void)
{
}

void Water::Draw(void)
{
	origShader_->Draw(lpModelMng.GetID(modelStr_)[modelNum_]);
}

void Water::Release(void)
{
}

int Water::GetModelNum(void)
{
	return modelNum_;
}

std::string Water::GetCollModelStr(void)
{
	return modelStr_;
}

void Water::VariableInit(void)
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
	type_ = GIMMICK_TYPE::WATER;

	//���f���p�X
	modelStr_ = "resource/model/gimmick/water.mv1";
}
