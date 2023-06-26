#include "JukeBox.h"
#include "../../common/manager/SoundMng.h"
#include "../../common/OriginalShader.h"

JukeBox::JukeBox(std::string name, Vector3 pos, Vector3 angle, Vector3 scale, int num)
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

JukeBox::~JukeBox()
{
	Release();
}

void JukeBox::Init(void)
{
	//変数初期化
	VariableInit();

	//座標を設定
	MV1SetPosition(lpModelMng.GetID(modelStr_)[modelNum_], pos_.toVECTOR());

	//拡大値を設定
	MV1SetScale(lpModelMng.GetID(modelStr_)[modelNum_], scale_.toVECTOR());

	//度数からラジアンに変換
	angle_ = { Deg2RadF(angle_.x),Deg2RadF(angle_.y),Deg2RadF(angle_.z) };

	//回転
	MV1SetRotationXYZ(lpModelMng.GetID(modelStr_)[modelNum_], angle_.toVECTOR());
	//ライティング
	vsLight_ = LoadVertexShader("./resource/shader/vs/NormalModel_vs.vso");
	psLight_ = LoadPixelShader("./resource/shader/ps/GameModel_ps.pso");
	//アウトライン
	int outL_vs = LoadVertexShader("./resource/shader/vs/OutLine_vs.vso");
	int outL_ps = LoadPixelShader("./resource/shader/ps/OutLine_ps.pso");
	origShader_ = std::make_unique<OriginalShader>(outL_vs, outL_ps);
	//lpSoundMng.Play3DSound("./resource/sound/test.wav", pos_, 6500.0f);
}

void JukeBox::Update(void)
{
	//Set3DPositionSoundMem(pos_.toVECTOR(), lpSoundMng.GetID("./resource/sound/test.wav")[0]);
	
	static std::string soundFileName = "./resource/sound/jukeBoxBGM.mp3";
	
	if (interactFlg_)
	{
		//音楽が流れているかチェック
		if (CheckSoundMem(lpSoundMng.GetID(soundFileName)[0]) == 1)
		{
			soundFlg_ = true;
		}
		else
		{
			soundFlg_ = false;
		}
	}

	// 流れている時
	if (soundFlg_)
	{
		// 音を止める
		lpSoundMng.StopSound(soundFileName);
	}

	// 止まっている時
	if (!soundFlg_)
	{
		// 音を流す
		lpSoundMng.Play3DSound(soundFileName, pos_, 15000.0f);
	}

	// 必ずinteractFlg_をfalseに
	interactFlg_ = false;
}

void JukeBox::Draw(void)
{
	origShader_->Draw(lpModelMng.GetID(modelStr_)[modelNum_], hitFlg_, psLight_, vsLight_);
}

void JukeBox::Release(void)
{
	//シェーダー削除
	DeleteShader(psLight_);
	DeleteShader(vsLight_);
}

int JukeBox::GetModelNum(void)
{
	return modelNum_;
}

std::string JukeBox::GetCollModelStr(void)
{
	return modelStr_;
}

void JukeBox::VariableInit(void)
{
	//インタラクトフラグ
	interactFlg_ = false;
	//サウンド再生フラグ
	soundFlg_ = true;
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
	type_ = GIMMICK_TYPE::JUKE_BOX;
	//モデルパス
	modelStr_ = "resource/model/gimmick/JukeBox.mv1";
}
