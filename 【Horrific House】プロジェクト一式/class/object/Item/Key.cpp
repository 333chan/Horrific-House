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
	//名前
	name_ = name;
	//初期位置
	homePos_ = bPos;
	//座標を初期位置に設定
	pos_ = homePos_;
	//移動後座標
	movedPos_ = aPos;
	//拡大値
	scale_ = scale;
	//角度
	angle_ = angle;
	//アイテムID
	itemId_ = keyID;
	//複製数
	modelNum_ = num;
	//初期化
	Init();
}

Key::~Key()
{
	Release();
}

void Key::Init(void)
{
	
	//移動フラグ
	moveFlg_ = false;
	//取得フラグ
	pickupFlg_ = false;
	//ロックフラグ
	lockFlg_ = true;
	//当たり判定フラグ
	hitFlg_ = false;
	//移動状態
	moveState_ = ItemMove::Close;
	//アニメーション状態
	animState_ = ItemAnim::Stop;
	//モデルをロード
	modelString_ = "resource/model/Item/old_key.mv1";
	//拡大値を設定
	MV1SetScale(lpModelMng.GetID(modelString_)[modelNum_], VECTOR(scale_.toVECTOR()));
	//配置
	MV1SetPosition(lpModelMng.GetID(modelString_)[modelNum_], pos_.toVECTOR());
	//度数からラジアンに変換
	angle_ = { Deg2RadF(angle_.x),Deg2RadF(angle_.y),Deg2RadF(angle_.z) };
	//回転
	MV1SetRotationXYZ(lpModelMng.GetID(modelString_)[modelNum_], angle_.toVECTOR());

	//ライティング
	vsLight_ = LoadVertexShader("./resource/shader/vs/NormalModel_vs.vso");
	psLight_ = LoadPixelShader("./resource/shader/ps/GameModel_ps.pso");
	//アウトライン
	int outL_vs = LoadVertexShader("./resource/shader/vs/OutLine_vs.vso");
	int outL_ps = LoadPixelShader("./resource/shader/ps/OutLine_ps.pso");
	origShader_ = std::make_unique<OriginalShader>(outL_vs, outL_ps);

	//カウンター
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
		//移動
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
	//当たり判定情報更新
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
	//シェーダー削除
	DeleteShader(psLight_);
	DeleteShader(vsLight_);
}

void Key::Move(void)
{
	animState_ = ItemAnim::Playing;
	if (animState_ == ItemAnim::Playing)
	{
		//閉まっている時
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
		//開いている時
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
