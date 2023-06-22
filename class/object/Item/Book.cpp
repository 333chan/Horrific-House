#include "Book.h"
#include "../../common/manager/ModelMng.h"
#include"../../common/OriginalShader.h"
constexpr float SPEED = 17.2f;

Book::Book(std::string name, Vector3 bPos, Vector3 aPos, Vector3 angle, Vector3 scale, ItemID bookID, int num)
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
	itemId_ = bookID;
	//複製数
	modelNum_ = num;
	//初期化
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
	//モデルをロード
	modelString_ = "resource/model/Item/book.mv1";

	//拡大値を設定
	MV1SetScale(lpModelMng.GetID(modelString_)[modelNum_], VECTOR(scale_.toVECTOR()));
	//拡大分座標を増やす
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

void Book::Update(void)
{
	//既に拾われていたら
	if (pickupFlg_)
	{
		//処理しない
		return;
	}
	//移動フラグがtrueなら
	if (moveFlg_)
	{
		//移動
		Move();
	}
	MV1SetPosition(lpModelMng.GetID(modelString_)[modelNum_], pos_.toVECTOR());
	//当たり判定情報更新
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
		//閉まっている時
		if (moveState_ == ItemMove::Close)
		{
			//座標が移動後より小さかったら
			if (pos_.x < movedPos_.x || pos_.y < movedPos_.y || pos_.z < movedPos_.z)
			{
				//正規化したベクトル方向に座標を増やす
				pos_ += (movedPos_ - pos_).Normalized() * SPEED;
			}
			//移動後座標を超えたら
			if (pos_ >= movedPos_)
			{
				//アニメーションを止める
				moveFlg_ = false;
				//ロックを解除
				lockFlg_ = false;
				//開いている状態
				moveState_ = ItemMove::Open;
				//アニメーションを停止状態
				animState_ = ItemAnim::Stop;
			}
		}
		//開いている時
		else if (moveState_ == ItemMove::Open)
		{
			//座標が初期座標より大きかったら
			if (pos_.x > homePos_.x || pos_.y > homePos_.y || pos_.z > homePos_.z)
			{
				//正規化したベクトル方向に座標を減らす
				pos_ -= (pos_ - homePos_).Normalized() * SPEED;
			}
			//初期座標を超えたら
			if (pos_ <= homePos_)
			{
				//アニメーションを止める
				moveFlg_ = false;
				//ロックをかける
				lockFlg_ = true;
				//閉じている状態状態
				moveState_ = ItemMove::Close;
				//アニメーションを停止状態
				animState_ = ItemAnim::Stop;
			}
		}
	}
}
