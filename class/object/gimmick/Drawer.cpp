#include <DxLib.h>
#include "Drawer.h"
#include "../../scene/SceneManager.h"
#include "../../common/AnimationController.h"
#include "../../scene/SceneManager.h"
#include "../../common/OriginalShader.h"
#include "../../common/manager/SoundMng.h"
constexpr float animSpeed = 35.0f;

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="座標"></param>
/// <param name="角度"></param>
/// <param name="オブジェクト名"></param>
/// <param name="複製数"></param>
Drawer::Drawer(std::string name, Vector3 pos, Vector3 angle, Vector3 scale, int num)
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

Drawer::~Drawer()
{
	Release();
}

//初期化
void Drawer::Init(void)
{
	
	//変数初期化
	VariableInit();

	//座標を設定
	MV1SetPosition(lpModelMng.GetID(modelStr_)[modelNum_], pos_.toVECTOR());
	
	//拡大値を設定
	MV1SetScale(lpModelMng.GetID(modelStr_)[modelNum_], VECTOR(scale_.toVECTOR()));
	
	//度数からラジアンに変換
	angle_ = { Deg2RadF(angle_.x),Deg2RadF(angle_.y),Deg2RadF(angle_.z) };
	
	//回転
	MV1SetRotationXYZ(lpModelMng.GetID(modelStr_)[modelNum_], angle_.toVECTOR());
	//ライティング
	vsLight_ = LoadVertexShader("./resource/shader/vs/NormalModel_vs.vso");
	psLight_ = LoadPixelShader("./resource/shader/ps/GameModel_ps.pso");
	//アウトライン
	origShader_ = std::make_unique<OriginalShader>(vsLight_, psLight_);

}

//更新
void Drawer::Update(void)
{
	//インタラクトされたら
	if (interactFlg_)
	{
		//再生する
		animFlg_ = true;
	}
	//アニメーション処理
	Animation();
	interactFlg_ = false;
}

//描画
void Drawer::Draw(void)
{
	//描画
	origShader_->Draw(lpModelMng.GetID(modelStr_)[modelNum_]);
}

//開放
void Drawer::Release(void)
{	
	//シェーダー削除
	DeleteShader(psLight_);
	DeleteShader(vsLight_);
}

int Drawer::GetModelNum(void)
{
	return modelNum_;
}

std::string Drawer::GetCollModelStr(void)
{
	return modelStr_;
}

void Drawer::VariableInit(void)
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
	animState_ = GIMMICK_ANIM::CLOSE;
	//ギミックタイプ
	type_ = GIMMICK_TYPE::DRAWER;
	//モデルパス
	modelStr_ = "resource/model/gimmick/drawer.mv1";
	//アニメーションフラグ
	animFlg_ = false;

	//アニメーション追加
	animController_ = std::make_unique<AnimationController>(lpModelMng.GetID(modelStr_)[modelNum_]);
	animController_->Add((int)GIMMICK_ANIM::OPEN, "resource/model/gimmick/anim/DrawerOpen.mv1", animSpeed);
	animController_->Add((int)GIMMICK_ANIM::CLOSE, "resource/model/gimmick/anim/DrawerClose.mv1", animSpeed);
}

void Drawer::Animation(void)
{

	//再生中
	if (animFlg_)
	{
		//閉じているとき
		if (animState_ == GIMMICK_ANIM::CLOSE)
		{
			lpSoundMng.PlayingSound("resource/sound/DrawerOpenSE.mp3");
			//開ける
			animController_->Play((int)GIMMICK_ANIM::OPEN, false);
			//アニメーションが停止したら
			if (animController_->GetPlayState() == ANIM_STATE::STOPPING)
			{
				//開いている状態に切り替える
				animState_ = GIMMICK_ANIM::OPEN;
				//再生フラグをを止める
				animFlg_ = false;
			}
		}
		else if (animState_ == GIMMICK_ANIM::OPEN)	//閉じているとき
		{
			lpSoundMng.PlayingSound("resource/sound/DrawerCloseSE.mp3");
			//閉じる
			animController_->Play((int)GIMMICK_ANIM::CLOSE, false);
			//アニメーションが停止したら
			if (animController_->GetPlayState() == ANIM_STATE::STOPPING)
			{
				//開いている状態に切り替える
				animState_ = GIMMICK_ANIM::CLOSE;
				//再生フラグをを止める
				animFlg_ = false;
			}
		}
	}
	//当たり判定情報更新
	MV1RefreshCollInfo(lpModelMng.GetID(modelStr_)[modelNum_]);
	MV1SetPosition(lpModelMng.GetID(modelStr_)[modelNum_], pos_.toVECTOR());
	//アニメーション更新処理
	animController_->Update(lpSceneMng.GetDeltaTime());
}
