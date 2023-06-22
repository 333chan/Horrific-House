#include "Memo.h"
#include "../../common/manager/ModelMng.h"
#include"../../common/OriginalShader.h"
constexpr float SPEED = 17.2f;

Memo::Memo(std::string name, Vector3 bPos, Vector3 aPos, Vector3 angle, Vector3 scale, ItemID memoID, int num)
{
	//���O
	name_ = name;
	//�����ʒu
	homePos_ = bPos;
	//���W�������ʒu�ɐݒ�
	pos_ = homePos_;
	//�ړ�����W
	movedPos_ = aPos;
	//�g��l
	scale_ = scale;
	//�p�x
	angle_ = angle;
	//�A�C�e��ID
	itemId_ = memoID;
	//������
	modelNum_ = num;
	//������
	Init();
}

Memo::~Memo()
{
	Release();
}

void Memo::Init(void)
{
	//�ړ��t���O
	moveFlg_ = false;
	//�擾�t���O
	pickupFlg_ = false;
	//���b�N�t���O
	lockFlg_ = false;
	//�����蔻��t���O
	hitFlg_ = false;
	//�ړ����
	moveState_ = ItemMove::Close;
	//�A�j���[�V�������
	animState_ = ItemAnim::Stop;
	//���f���̃t�@�C���p�X
	modelString_ = "resource/model/Item/memo1.mv1";
	//�}�b�v�������ꍇ
	if (name_ == "map")
	{
		modelString_ = "resource/model/Item/map.mv1";
	}

	//�g��l��ݒ�
	MV1SetScale(lpModelMng.GetID(modelString_)[modelNum_], scale_.toVECTOR());
	//�z�u
	MV1SetPosition(lpModelMng.GetID(modelString_)[modelNum_], homePos_.toVECTOR());
	//��]
	angle_ = { Deg2RadF(angle_.x),Deg2RadF(angle_.y),Deg2RadF(angle_.z) };
	MV1SetRotationXYZ(lpModelMng.GetID(modelString_)[modelNum_], angle_.toVECTOR());

	//���C�e�B���O
	vsLight_ = LoadVertexShader("./resource/shader/vs/NormalModel_vs.vso");
	psLight_ = LoadPixelShader("./resource/shader/ps/GameModel_ps.pso");

	//�A�E�g���C��
	int outL_vs = LoadVertexShader("./resource/shader/vs/OutLine_vs.vso");
	int outL_ps = LoadPixelShader("./resource/shader/ps/OutLine_ps.pso");

	origShader_ = std::make_unique<OriginalShader>(outL_vs, outL_ps);
}

void Memo::Update(void)
{
	if (pickupFlg_)
	{
		return;
	}

	if (moveFlg_)
	{
		//�ړ�
		Move();
	}
	//���W�X�V
	MV1SetPosition(lpModelMng.GetID(modelString_)[modelNum_], pos_.toVECTOR());
	//�����蔻����X�V
	MV1RefreshCollInfo(lpModelMng.GetID(modelString_)[modelNum_]);
}

void Memo::Draw(void)
{
}

void Memo::Draw(bool cameraflag)
{
	if (!pickupFlg_)
	{
		origShader_->Draw(lpModelMng.GetID(modelString_)[modelNum_], hitFlg_, psLight_, vsLight_);
	}
}


void Memo::Release(void)
{
	//�V�F�[�_�[�폜
	DeleteShader(psLight_);
	DeleteShader(vsLight_);
}

void Memo::Move(void)
{
	animState_ = ItemAnim::Playing;
	if (animState_ == ItemAnim::Playing)
	{
		//�܂��Ă��鎞
		if (moveState_ == ItemMove::Close)
		{
			if (pos_.x > movedPos_.x || pos_.y < movedPos_.y || pos_.z < movedPos_.z)
			{
				pos_ += (movedPos_ - pos_).Normalized() * SPEED;
			}
			if (pos_ <= movedPos_)
			{
				moveFlg_ = false;
				lockFlg_ = false;
				moveState_ = ItemMove::Open;
				animState_ = ItemAnim::Stop;
			}
		}
		//�J���Ă��鎞
		else if (moveState_ == ItemMove::Open)
		{
			if (pos_.x < homePos_.x || pos_.y > homePos_.y || pos_.z > homePos_.z)
			{
				pos_ -= (pos_ - homePos_).Normalized() * SPEED;
			}
			if (pos_ >= homePos_)
			{
				moveFlg_ = false;
				lockFlg_ = true;
				moveState_ = ItemMove::Close;
				animState_ = ItemAnim::Stop;
			}
		}
	}
}
