#include "Memo.h"
#include "../../common/manager/ModelMng.h"
#include"../../common/OriginalShader.h"
constexpr float SPEED = 17.2f;

Memo::Memo(std::string name, Vector3 bPos, Vector3 aPos, Vector3 angle, Vector3 scale, ItemID memoID, int num)
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
	itemId_ = memoID;
	//複製数
	modelNum_ = num;
	//初期化
	Init();
}

Memo::~Memo()
{
	Release();
}

void Memo::Init(void)
{
	//移動フラグ
	moveFlg_ = false;
	//取得フラグ
	pickupFlg_ = false;
	//ロックフラグ
	lockFlg_ = false;
	//当たり判定フラグ
	hitFlg_ = false;
	//移動状態
	moveState_ = ItemMove::Close;
	//アニメーション状態
	animState_ = ItemAnim::Stop;
	//モデルのファイルパス
	modelString_ = "resource/model/Item/memo1.mv1";
	//マップだった場合
	if (name_ == "map")
	{
		modelString_ = "resource/model/Item/map.mv1";
	}

	//拡大値を設定
	MV1SetScale(lpModelMng.GetID(modelString_)[modelNum_], scale_.toVECTOR());
	//配置
	MV1SetPosition(lpModelMng.GetID(modelString_)[modelNum_], homePos_.toVECTOR());
	//回転
	angle_ = { Deg2RadF(angle_.x),Deg2RadF(angle_.y),Deg2RadF(angle_.z) };
	MV1SetRotationXYZ(lpModelMng.GetID(modelString_)[modelNum_], angle_.toVECTOR());

	//ライティング
	vsLight_ = LoadVertexShader("./resource/shader/vs/NormalModel_vs.vso");
	psLight_ = LoadPixelShader("./resource/shader/ps/GameModel_ps.pso");

	//アウトライン
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
		//移動
		Move();
	}
	//座標更新
	MV1SetPosition(lpModelMng.GetID(modelString_)[modelNum_], pos_.toVECTOR());
	//当たり判定情報更新
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
	//シェーダー削除
	DeleteShader(psLight_);
	DeleteShader(vsLight_);
}

void Memo::Move(void)
{
	animState_ = ItemAnim::Playing;
	if (animState_ == ItemAnim::Playing)
	{
		//閉まっている時
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
		//開いている時
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
