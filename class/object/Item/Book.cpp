#include "Book.h"
#include "../../common/manager/ModelMng.h"
#include"../../common/OriginalShader.h"
constexpr float SPEED = 17.2f;

Book::Book(std::string name, Vector3 bPos, Vector3 aPos, Vector3 angle, Vector3 scale, ItemID bookID, int num)
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
	itemId_ = bookID;
	//������
	modelNum_ = num;
	//������
	Init();
}

Book::~Book()
{
}

void Book::Init(void)
{
	moveFlg_ = false;
	pickupFlg_ = false;
	lockFlg_ = false;
	hitFlg_ = false;
	moveState_ = ItemMove::Close;
	animState_ = ItemAnim::Stop;
	//���f�������[�h
	modelString_ = "resource/model/Item/book.mv1";

	//�g��l��ݒ�
	MV1SetScale(lpModelMng.GetID(modelString_)[modelNum_], VECTOR(scale_.toVECTOR()));
	//�g�啪���W�𑝂₷
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

void Book::Update(void)
{
	//���ɏE���Ă�����
	if (pickupFlg_)
	{
		//�������Ȃ�
		return;
	}
	//�ړ��t���O��true�Ȃ�
	if (moveFlg_)
	{
		//�ړ�
		Move();
	}
	MV1SetPosition(lpModelMng.GetID(modelString_)[modelNum_], pos_.toVECTOR());
	//�����蔻����X�V
	MV1RefreshCollInfo(lpModelMng.GetID(modelString_)[modelNum_]);
}

void Book::Draw(void)
{
}

void Book::Draw(bool cameraflag)
{
	if (!pickupFlg_)
	{
		origShader_->Draw(lpModelMng.GetID(modelString_)[modelNum_], hitFlg_, psLight_, vsLight_);
	}
}

void Book::Release(void)
{
}

void Book::Move(void)
{
	animState_ = ItemAnim::Playing;
	if (animState_ == ItemAnim::Playing)
	{
		//�܂��Ă��鎞
		if (moveState_ == ItemMove::Close)
		{
			//���W���ړ����菬����������
			if (pos_.x < movedPos_.x || pos_.y < movedPos_.y || pos_.z < movedPos_.z)
			{
				//���K�������x�N�g�������ɍ��W�𑝂₷
				pos_ += (movedPos_ - pos_).Normalized() * SPEED;
			}
			//�ړ�����W�𒴂�����
			if (pos_ >= movedPos_)
			{
				//�A�j���[�V�������~�߂�
				moveFlg_ = false;
				//���b�N������
				lockFlg_ = false;
				//�J���Ă�����
				moveState_ = ItemMove::Open;
				//�A�j���[�V�������~���
				animState_ = ItemAnim::Stop;
			}
		}
		//�J���Ă��鎞
		else if (moveState_ == ItemMove::Open)
		{
			//���W���������W���傫��������
			if (pos_.x > homePos_.x || pos_.y > homePos_.y || pos_.z > homePos_.z)
			{
				//���K�������x�N�g�������ɍ��W�����炷
				pos_ -= (pos_ - homePos_).Normalized() * SPEED;
			}
			//�������W�𒴂�����
			if (pos_ <= homePos_)
			{
				//�A�j���[�V�������~�߂�
				moveFlg_ = false;
				//���b�N��������
				lockFlg_ = true;
				//���Ă����ԏ��
				moveState_ = ItemMove::Close;
				//�A�j���[�V�������~���
				animState_ = ItemAnim::Stop;
			}
		}
	}
}
