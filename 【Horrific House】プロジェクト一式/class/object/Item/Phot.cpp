#include "Phot.h"
#include "../../common/manager/ModelMng.h"
#include"../../common/OriginalShader.h"
Phot::Phot(std::string name, Vector3 bPos, Vector3 aPos, Vector3 angle, Vector3 scale, ItemID id, int num)
{
	name_ = name;
	homePos_ = bPos;
	homePos_ = aPos;
	angle_ = angle;
	scale_ = scale;
	itemId_ = id;
	modelNum_ = num;
	Init();
}

//Phot::Phot(Vector3 pos)
//{
//	pos_ = pos;
//	itemId_ = ItemID::Phot;
//	modelNum_ = 0;
//}

Phot::~Phot()
{
	Release();
}

void Phot::Init(void)
{
	//�ړ��t���O
	moveFlg_ = false;

	//�擾�t���O
	pickupFlg_ = false;

	//���b�N�t���O
	lockFlg_ = false;

	//�����蔻��{���u
	hitFlg_ = false;

	//���f�������[�h
	modelString_ = "resource/model/Item/phot.mv1";

	//�g��l��ݒ�
	MV1SetScale(lpModelMng.GetID(modelString_)[modelNum_], scale_.toVECTOR());
	//�z�u
	MV1SetPosition(lpModelMng.GetID(modelString_)[modelNum_], homePos_.toVECTOR());
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

}

void Phot::Update(void)
{
	if (pickupFlg_)
	{
		return;
	}
}

void Phot::Draw(void)
{
	//if (!pickupFlg_)
	//{
	//	MV1DrawModel(lpModelMng.GetID(modelString_)[modelNum_]);
	//}

}

void Phot::Draw(bool cameraflag)
{
	if (!pickupFlg_)
	{
		origShader_->Draw(lpModelMng.GetID(modelString_)[modelNum_], hitFlg_, psLight_, vsLight_);
	}
}

void Phot::Release(void)
{
	//���
	//MV1DeleteModel(lpModelMng.GetID(modelString_)[0]);
	DeleteShader(psLight_);
	DeleteShader(vsLight_);
}
