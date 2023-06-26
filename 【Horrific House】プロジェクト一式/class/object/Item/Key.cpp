#include "Key.h"
#include "../../common/manager/ModelMng.h"
#include"../../common/OriginalShader.h"
#include "../../../_debug/_DebugDispOut.h"
#include "../../../_debug/_DebugConOut.h"
constexpr float SIZE_SCALE = 10.0f;
constexpr float ADD_DEG_ANGLE = 1.0f;

constexpr float SIN_PERIOD = 600.0f;
constexpr float MOVE_POWER = 100.0f;
constexpr float SPEED = 17.2f;

Key::Key(std::string name, Vector3 bPos, Vector3 aPos ,Vector3 angle, Vector3 scale,ItemID keyID,int num)
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
	itemId_ = keyID;
	//������
	modelNum_ = num;
	//������
	Init();
}

Key::~Key()
{
	Release();
}

void Key::Init(void)
{
	
	//�ړ��t���O
	moveFlg_ = false;
	//�擾�t���O
	pickupFlg_ = false;
	//���b�N�t���O
	lockFlg_ = true;
	//�����蔻��t���O
	hitFlg_ = false;
	//�ړ����
	moveState_ = ItemMove::Close;
	//�A�j���[�V�������
	animState_ = ItemAnim::Stop;
	//���f�������[�h
	modelString_ = "resource/model/Item/old_key.mv1";
	//�g��l��ݒ�
	MV1SetScale(lpModelMng.GetID(modelString_)[modelNum_], VECTOR(scale_.toVECTOR()));
	//�z�u
	MV1SetPosition(lpModelMng.GetID(modelString_)[modelNum_], pos_.toVECTOR());
	//�x�����烉�W�A���ɕϊ�
	angle_ = { Deg2RadF(angle_.x),Deg2RadF(angle_.y),Deg2RadF(angle_.z) };
	//��]
	MV1SetRotationXYZ(lpModelMng.GetID(modelString_)[modelNum_], angle_.toVECTOR());

	//���C�e�B���O
	vsLight_ = LoadVertexShader("./resource/shader/vs/NormalModel_vs.vso");
	psLight_ = LoadPixelShader("./resource/shader/ps/GameModel_ps.pso");
	//�A�E�g���C��
	int outL_vs = LoadVertexShader("./resource/shader/vs/OutLine_vs.vso");
	int outL_ps = LoadPixelShader("./resource/shader/ps/OutLine_ps.pso");
	origShader_ = std::make_unique<OriginalShader>(outL_vs, outL_ps);

	//�J�E���^�[
	count_ = 0.0f;
	switch (itemId_)
	{
	case ItemID::Celler_Key:
		lockFlg_ = true;
		break;
	default:
		lockFlg_ = false;
		break;
	}
}

void Key::Update(void)
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
	switch (itemId_)
	{
	case ItemID::Entrance_Key:
	case ItemID::Kitchen_Key:
	case ItemID::FoodBank_Key:
	case ItemID::ChildRoom_Key:
		angle_.y += Deg2RadF(ADD_DEG_ANGLE);
		MV1SetPosition(lpModelMng.GetID(modelString_)[modelNum_], VGet(pos_.x, pos_.y + SinCurve(SIN_PERIOD, count_,MOVE_POWER), pos_.z));
		MV1SetRotationXYZ(lpModelMng.GetID(modelString_)[modelNum_], angle_.toVECTOR());
		count_ += 1.0f;
		break;
	case ItemID::MusicRoom_Key:
	case ItemID::Celler_Key:
		MV1SetPosition(lpModelMng.GetID(modelString_)[modelNum_], pos_.toVECTOR());
		break;
	default:
		MV1SetPosition(lpModelMng.GetID(modelString_)[modelNum_], pos_.toVECTOR());
		break;
	}
	//�����蔻����X�V
	MV1RefreshCollInfo(lpModelMng.GetID(modelString_)[modelNum_]);
}

void Key::Draw(void)
{
	if (!pickupFlg_) 
	{
		MV1DrawModel(lpModelMng.GetID(modelString_)[modelNum_]);
	}
}

void Key::Draw(bool cameraflag)
{
	if (!pickupFlg_ && cameraflag)
	{
		if (itemId_ != ItemID::Celler_Key)
		{
			origShader_->Draw(lpModelMng.GetID(modelString_)[modelNum_],hitFlg_,psLight_,vsLight_);
			//origShader_->Draw(lpModelMng.GetID(modelString_)[modelNum_]);
		}
	}
	if (!pickupFlg_ && itemId_ == ItemID::Celler_Key)
	{
		origShader_->Draw(lpModelMng.GetID(modelString_)[modelNum_], hitFlg_, psLight_, vsLight_);
	}
}

void Key::Release(void)
{
	//�V�F�[�_�[�폜
	DeleteShader(psLight_);
	DeleteShader(vsLight_);
}

void Key::Move(void)
{
	animState_ = ItemAnim::Playing;
	if (animState_ == ItemAnim::Playing)
	{
		//�܂��Ă��鎞
		if (moveState_ == ItemMove::Close)
		{
			if (pos_.x < movedPos_.x || pos_.y < movedPos_.y || pos_.z < movedPos_.z)
			{
				pos_ += (movedPos_ - pos_).Normalized() * SPEED;
			}
			if (pos_ >= movedPos_)
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
			if (pos_.x > homePos_.x || pos_.y > homePos_.y || pos_.z > homePos_.z)
			{
				pos_ -= (pos_ - homePos_).Normalized() * SPEED;
			}
			if (pos_ <= homePos_)
			{
				moveFlg_ = false;
				lockFlg_ = true;
				moveState_ = ItemMove::Close;
				animState_ = ItemAnim::Stop;
			}
		}
	}
}
