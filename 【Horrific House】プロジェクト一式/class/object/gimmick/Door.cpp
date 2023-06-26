#include "Door.h"
#include "../../common/AnimationController.h"
#include "../../Scene/SceneManager.h"
#include "../../common/OriginalShader.h"
#include "../../common/manager/SoundMng.h"
using namespace std;
constexpr float OPEN_SPEED = 18.0f;
constexpr float CLOSE_SPEED = 17.0f;


Door::Door(std::string name, Vector3 pos, Vector3 angle, Vector3 scale, int num)
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

Door::~Door()
{
	Release();
}

void Door::Init(void)
{

	//変数初期化
	VariableInit();

	//座標を設定
	MV1SetPosition(lpModelMng.GetID(modelStr_)[modelNum_],pos_.toVECTOR());
	
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
	int outL_vs = LoadVertexShader("./resource/shader/vs/OutLine_vs.vso");
	int outL_ps = LoadPixelShader("./resource/shader/ps/OutLine_ps.pso");
	origShader_ = std::make_unique<OriginalShader>(outL_vs, outL_ps);
}

void Door::Update(void)
{
	//インタラクトされたら
	if (interactFlg_)
	{
		//アニメーション再生
		animFlg_ = true;
	}

	//アニメーション処理
	Animation();
	interactFlg_ = false;
}

void Door::Draw(void)
{
	//描画
	origShader_->Draw(lpModelMng.GetID(modelStr_)[modelNum_], hitFlg_, psLight_, vsLight_);
	//MV1DrawModelDebug(lpModelMng.GetID("./resource/model/gimmick/Door_White.mv1")[num_],0xffffff,1,0.001,1,1);
}

void Door::Release(void)
{
	//シェーダー削除
	DeleteShader(psLight_);
	DeleteShader(vsLight_);
}

int Door::GetModelNum(void)
{
	return modelNum_;
}

std::string Door::GetCollModelStr(void)
{
	return modelStr_;
}

void Door::Animation(void)
{
	//再生中
	if (animFlg_)
	{		
		//閉じているとき
		if (animState_ == GIMMICK_ANIM::CLOSE)
		{
			lpSoundMng.PlayingSound("resource/sound/DoorSE.mp3");
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
		else if (animState_ == GIMMICK_ANIM::OPEN)	//開いているとき
		{
			//
			lpSoundMng.PlayingSound("resource/sound/DoorSE.mp3");
			//閉じる
			animController_->Play((int)GIMMICK_ANIM::CLOSE, false);
			//アニメーションが停止したら
			if (animController_->GetPlayState() == ANIM_STATE::STOPPING)
			{
				//閉じている状態に切り替える
				animState_ = GIMMICK_ANIM::CLOSE;
				//再生フラグをを止める
				animFlg_ = false;
				lpSoundMng.PlayingSound("resource/sound/DoorCloseSE.mp3");
			}
		}
	}
	//当たり判定情報更新
	MV1RefreshCollInfo(lpModelMng.GetID(modelStr_)[modelNum_]);
	//アニメーション更新処理
	animController_->Update(lpSceneMng.GetDeltaTime());
}

void Door::VariableInit(void)
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
	//アニメーションフラグ
	animFlg_ = false;
	//アニメーション種類
	animState_ = GIMMICK_ANIM::CLOSE;
	//ギミックタイプ
	type_ = GIMMICK_TYPE::DOOR;
	//ファイルのディレクトリ
	std::string directory = "resource/model/gimmick/";
	//使用するモデルを設定
	if (name_ == "Dark_A_Double")
	{
		modelStr_ = directory+"Door_Dark1_Double.mv1";

		//アニメーション追加
		animController_ = std::make_unique<AnimationController>(lpModelMng.GetID(modelStr_)[modelNum_]);
		animController_->Add((int)GIMMICK_ANIM::OPEN, "./resource/model/gimmick/anim/DoorDoubleOpen.mv1", OPEN_SPEED);
		animController_->Add((int)GIMMICK_ANIM::CLOSE, "./resource/model/gimmick/anim/DoorDoubleClose.mv1", CLOSE_SPEED);
	}
	else
	{
		modelStr_ = directory + "Door_Dark1.mv1";

		//アニメーション追加
		animController_ = std::make_unique<AnimationController>(lpModelMng.GetID(modelStr_)[modelNum_]);
		animController_->Add((int)GIMMICK_ANIM::OPEN, "./resource/model/gimmick/anim/DoorOpen.mv1", OPEN_SPEED);
		animController_->Add((int)GIMMICK_ANIM::CLOSE, "./resource/model/gimmick/anim/DoorClose.mv1", CLOSE_SPEED);
	}
}


