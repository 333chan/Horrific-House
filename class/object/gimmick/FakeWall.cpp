#include "FakeWall.h"
#include "../../common/OriginalShader.h"

FakeWall::FakeWall(std::string name, Vector3 pos, Vector3 angle, Vector3 scale, int num, ItemID itemID)
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
	//�t���O�ƂȂ�A�C�e��ID
	itemID_	= itemID;
	//������
	Init();
}

FakeWall::~FakeWall()
{
}

void FakeWall::Init(void)
{
	//�ϐ�������
	VariableInit();

	//���W��ݒ�
	MV1SetPosition(lpModelMng.GetID(modelStr_)[modelNum_], pos_.toVECTOR());

	//�g��l�ݒ�
	scale_ = { 1.01f,1.0f,1.0f };
	MV1SetScale(lpModelMng.GetID(modelStr_)[modelNum_], VECTOR(scale_.toVECTOR()));

	//�x���󃉃W�A���ɕϊ�
	angle_ = { Deg2RadF(angle_.x),Deg2RadF(angle_.y),Deg2RadF(angle_.z) };

	//��]
	MV1SetRotationXYZ(lpModelMng.GetID(modelStr_)[modelNum_], angle_.toVECTOR());

	//���C�e�B���O
	int VertexShader = LoadVertexShader("resource/shader/vs/NormalModel_vs.vso");
	int PixelShader = LoadPixelShader("resource/shader/ps/GameModel_ps.pso");
	origShader_ = std::make_unique<OriginalShader>(VertexShader, PixelShader);
}

void FakeWall::Update(void)
{
}

void FakeWall::Draw(void)
{
	if (isLock_)
	{
		origShader_->Draw(lpModelMng.GetID(modelStr_)[modelNum_]);
	}
}

void FakeWall::Release(void)
{
}

int FakeWall::GetModelNum(void)
{
    return modelNum_;
}

std::string FakeWall::GetCollModelStr(void)
{
    return modelStr_;
}

void FakeWall::VariableInit(void)
{
	//�C���^���N�g�t���O
	interactFlg_ = false;
	//�_�C�A���i���o�[
	dialNumber_ = 0;
	//���b�N���
	isLock_ = true;
	//�����蔻��t���O
	hitFlg_ = false;
	//�A�����b�N�i���o�[
	unlockNum_ = "0000";
	//�����蔻��p�@��
	hitNormal_ = { 0.0f,0.0f,0.0f };
	//�A�j���[�V�������
	animState_ = GIMMICK_ANIM::NONE;
	//�M�~�b�N�^�C�v
	type_ = GIMMICK_TYPE::WALL;
	//���f���p�X
	modelStr_ = "resource/model/gimmick/Fake_Wall.mv1";
}
