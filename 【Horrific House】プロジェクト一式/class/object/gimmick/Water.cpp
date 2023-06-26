#include "Water.h"
#include "../../common/OriginalShader.h"

constexpr float DIV_VALUE = 1000.0f;

Water::Water(std::string name, Vector3 pos, Vector3 angle, Vector3 scale, int num)
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
	//初期化
	Init();
}

Water::~Water()
{
}

void Water::Init(void)
{
	//変数初期化
	VariableInit();
	//座標を設定
	MV1SetPosition(lpModelMng.GetID(modelStr_)[modelNum_], pos_.toVECTOR());

	//拡大値を設定
	scale_ = { scale_.x / DIV_VALUE,scale_.y / DIV_VALUE,scale_.z / DIV_VALUE };
	MV1SetScale(lpModelMng.GetID(modelStr_)[modelNum_], scale_.toVECTOR());

	//度数からラジアンに変換
	angle_ = { Deg2RadF(angle_.x),Deg2RadF(angle_.y),Deg2RadF(angle_.z) };
	//回転
	MV1SetRotationXYZ(lpModelMng.GetID(modelStr_)[modelNum_], angle_.toVECTOR());
	//水
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
	//インタラクトフラグ
	interactFlg_ = false;

	//ダイアルナンバー
	dialNumber_ = 0;

	//ロック状態
	isLock_ = false;

	//当たり判定フラグ
	hitFlg_ = false;

	//アンロックナンバー
	unlockNum_ = "0000";

	//アイテムID
	itemID_ = ItemID(0);

	//当たり判定用法線
	hitNormal_ = { 0.0f,0.0f,0.0f };

	//アニメーション種類
	animState_ = GIMMICK_ANIM::NONE;

	//ギミックタイプ
	type_ = GIMMICK_TYPE::WATER;

	//モデルパス
	modelStr_ = "resource/model/gimmick/water.mv1";
}
