#include "Dial.h"
#include "../../common/AnimationController.h"
#include "../../common/OriginalShader.h"
#include "../../common/manager/SoundMng.h"
constexpr float STOP_ANGLE = 36.0f;
constexpr float MAX_ANGLE = 360.0f;
constexpr float ROT_SPEED = 1.5f;

Dial::Dial(std::string name, Vector3 pos, Vector3 angle, Vector3 scale, int num)
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

Dial::~Dial()
{
	Release();
}

void Dial::Init(void)
{
	//変数初期化
	VariableInit();

	//位置設定
	MV1SetPosition(lpModelMng.GetID(modelStr_)[modelNum_],pos_.toVECTOR());

	//拡大値設定
	MV1SetScale(lpModelMng.GetID(modelStr_)[modelNum_], VECTOR(scale_.toVECTOR()));

	//度数からラジアンに変換
	angle_ = { Deg2RadF(angle_.x),Deg2RadF(angle_.y),Deg2RadF(angle_.z) };

	//回転
	MV1SetRotationXYZ(lpModelMng.GetID(modelStr_)[modelNum_], angle_.toVECTOR());

	//ライティング
	vsLight_ = LoadVertexShader("./resource/shader/vs/NormalModel_vs.vso");
	psLight_ = LoadPixelShader("./resource/shader/ps/GameModel_ps.pso");

	//シェーダー設定
	origShader_ = std::make_unique<OriginalShader>(vsLight_, psLight_);
}

void Dial::Update(void)
{
	//インタラクトされたら
	if (interactFlg_)
	{
		//アニメーション再生
		animFlg_ = true;
	}

	//回転
	Rotaion();
}

//描画
void Dial::Draw(void)
{
	origShader_->Draw(lpModelMng.GetID(modelStr_)[modelNum_]);
}

//開放
void Dial::Release(void)
{
	//シェーダー削除
	DeleteShader(psLight_);
	DeleteShader(vsLight_);
}
//モデル番号を返す
int Dial::GetModelNum(void)
{
	return modelNum_;
}
//モデルも文字列を返す
std::string Dial::GetCollModelStr(void)
{
	return modelStr_;
}

void Dial::VariableInit(void)
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
	type_ = GIMMICK_TYPE::DIAL;

	//停止角度
	stopAngle_ = 0.0f;

	//モデルパス
	modelStr_ = "resource/model/gimmick/Dial.mv1";
}

void Dial::Rotaion(void)
{
	//停止中
	if (!animFlg_)
	{
		//停止位置を決める
		stopAngle_ = angle_.x + Deg2RadF(STOP_ANGLE);

		//angle_が360度になったら
		if (angle_.x >= Deg2RadF(MAX_ANGLE))
		{
			//angle_の値が増えすぎないよう0に戻す
			angle_.x = 0.0f;
		}
		//現在のダイヤル番号(現在の角度/停止位置)
		dialNumber_ = static_cast<int>(angle_.x / Deg2RadF(STOP_ANGLE));
	}
	else	//進行中
	{
		lpSoundMng.PlaySoundOneTime("resource/sound/DialSE.mp3");
		//angle_が停止角度より小さいなら
		if(angle_.x <= stopAngle_)
		{
			//回転
			angle_.x += Deg2RadF(ROT_SPEED);
		}
		else	//停止角度を超えたら
		{
			lpSoundMng.ResetCnt("resource/sound/DialSE.mp3");
			//停止する
			animFlg_ = false;
		}
		interactFlg_ = false;
	}
	//回転更新
	MV1SetRotationXYZ(lpModelMng.GetID(modelStr_)[modelNum_], angle_.toVECTOR());
}
