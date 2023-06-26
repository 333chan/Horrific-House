#include "HiddenText.h"
#include "../../common/OriginalShader.h"

HiddenText::HiddenText(std::string name, Vector3 pos, Vector3 angle, Vector3 scale, int num)
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

HiddenText::~HiddenText()
{
}

void HiddenText::Init(void)
{
	//変数初期化
	VariableInit();

	//座標を設定
	MV1SetPosition(lpModelMng.GetID(modelStr_)[modelNum_], pos_.toVECTOR());

	//拡大値を設定
	scale_ = { 10.0f,10.0f,1.0f };
	MV1SetScale(lpModelMng.GetID(modelStr_)[modelNum_], VECTOR(scale_.toVECTOR()));

	//度数からラジアンに変換
	angle_ = { Deg2RadF(angle_.x),Deg2RadF(angle_.y),Deg2RadF(angle_.z) };

	//回転
	MV1SetRotationXYZ(lpModelMng.GetID(modelStr_)[modelNum_], angle_.toVECTOR());

	//ライティング
	int VertexShader = LoadVertexShader("resource/shader/vs/NormalModel_vs.vso");
	int PixelShader = LoadPixelShader("resource/shader/ps/GameModel_ps.pso");
	origShader_ = std::make_unique<OriginalShader>(VertexShader, PixelShader);
}

void HiddenText::Update(void)
{
}

void HiddenText::Draw(void)
{
	if (!isLock_)
	{
		//描画
		origShader_->Draw(lpModelMng.GetID(modelStr_)[modelNum_]);
	}
}

void HiddenText::Release(void)
{
}

int HiddenText::GetModelNum(void)
{
	return modelNum_;
}

std::string HiddenText::GetCollModelStr(void)
{
	return modelStr_;
}

void HiddenText::VariableInit(void)
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
	//使用するモデル設定
	if (name_ == "firstHint")
	{
		modelStr_ = "resource/model/gimmick/firstHint.mv1";
	}
	else if (name_ == "secondHint")
	{
		modelStr_ = "resource/model/gimmick/secondHint.mv1";
	}
	//ギミックタイプ
	type_ = GIMMICK_TYPE::TEXT;
}
