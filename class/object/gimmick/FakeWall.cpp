#include "FakeWall.h"
#include "../../common/OriginalShader.h"

FakeWall::FakeWall(std::string name, Vector3 pos, Vector3 angle, Vector3 scale, int num, ItemID itemID)
{
	//オブジェクト名
	name_ = name;
	//座標
	pos_ = pos;
	//角度
	angle_ = angle;
	//拡大値
	scale_ = scale;
	//モデル複製数
	modelNum_ = num;
	//フラグとなるアイテムID
	itemID_	= itemID;
	//初期化
	Init();
}

FakeWall::~FakeWall()
{
}

void FakeWall::Init(void)
{
	//変数初期化
	VariableInit();

	//座標を設定
	MV1SetPosition(lpModelMng.GetID(modelStr_)[modelNum_], pos_.toVECTOR());

	//拡大値設定
	scale_ = { 1.01f,1.0f,1.0f };
	MV1SetScale(lpModelMng.GetID(modelStr_)[modelNum_], VECTOR(scale_.toVECTOR()));

	//度数空ラジアンに変換
	angle_ = { Deg2RadF(angle_.x),Deg2RadF(angle_.y),Deg2RadF(angle_.z) };

	//回転
	MV1SetRotationXYZ(lpModelMng.GetID(modelStr_)[modelNum_], angle_.toVECTOR());

	//ライティング
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
	//インタラクトフラグ
	interactFlg_ = false;
	//ダイアルナンバー
	dialNumber_ = 0;
	//ロック状態
	isLock_ = true;
	//当たり判定フラグ
	hitFlg_ = false;
	//アンロックナンバー
	unlockNum_ = "0000";
	//当たり判定用法線
	hitNormal_ = { 0.0f,0.0f,0.0f };
	//アニメーション種類
	animState_ = GIMMICK_ANIM::NONE;
	//ギミックタイプ
	type_ = GIMMICK_TYPE::WALL;
	//モデルパス
	modelStr_ = "resource/model/gimmick/Fake_Wall.mv1";
}
