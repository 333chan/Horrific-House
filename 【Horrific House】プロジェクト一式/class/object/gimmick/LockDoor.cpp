#include "LockDoor.h"
#include "../../common/AnimationController.h"
#include "../../scene/SceneManager.h"
#include "../Item/Item.h"
#include "../../common/OriginalShader.h"
#include "../../common/manager/SoundMng.h"
constexpr double UI_DRAW_TIME = 1.3;		//UI表示時間
constexpr int OFFSET_Y = 370;		//UI表示時間
constexpr float OPEN_SPEED = 18.0f;
constexpr float CLOSE_SPEED = 17.0f;

LockDoor::LockDoor(std::string name, Vector3 pos, Vector3 angle, Vector3 scale, int num, ItemID itemID)
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
	//開けるために必要なアイテム
	itemID_ = itemID;
	//初期化
	Init();
}

LockDoor::~LockDoor()
{
	Release();
}

void LockDoor::Init(void)
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
}

void LockDoor::Update(void)
{
	//インタラクトされたら
	if (interactFlg_)
	{
		//ロック中
		if (isLock_)
		{
			//ガチャガチャ音を再生
			lpSoundMng.PlayingSound("resource/sound/LockDoorSE.mp3");
		}
		else	//ロック解除されていたら
		{
			//アニメーション再生
			animFlg_ = true;
		}
		//UI表示
		uiFlg_ = true;
	}
	//アニメーション処理
	Animation();
	//描画時間計算
	DrawTimeCount();
	interactFlg_ = false;
}

void LockDoor::Draw(void)
{
	//描画
	origShader_->Draw(lpModelMng.GetID(modelStr_)[modelNum_], hitFlg_,psLight_,vsLight_);
	//UI描画
	DrawUI();
}

void LockDoor::Release(void)
{
	//シェーダー削除
	DeleteShader(psLight_);
	DeleteShader(vsLight_);
}

int LockDoor::GetModelNum(void)
{
	return 	modelNum_;
}

std::string LockDoor::GetCollModelStr(void)
{
	return modelStr_;
}


void LockDoor::Animation(void)
{
	//ロックが解除
	if (!isLock_)
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
	}
	//当たり判定情報更新
	MV1RefreshCollInfo(lpModelMng.GetID(modelStr_)[modelNum_]);
	//アニメーション更新処理
	animController_->Update(lpSceneMng.GetDeltaTime());
}

void LockDoor::DrawTimeCount(void)
{
	//表示中
	if (uiFlg_)
	{
		delta_ += lpSceneMng.GetDeltaTime();
	}
	else	//非表示中
	{
		//時間を初期化
		delta_ = 0.0;
	}

	//経過時間が指定した時間を超えたら
	if (delta_ >= UI_DRAW_TIME)
	{
		//UIを消す
		uiFlg_ = false;
		//ロックが解除されていたら
		if (!isLock_)
		{
			//解除UIを1回だけ表示するためフラグをfalseに
			firstTimeFlg_ = false;
		}
	}
}

void LockDoor::DrawUI(void)
{	
	if (uiFlg_ && firstTimeFlg_)
	{
		//画像のサイズを設定
		SetGraphSize();
		DrawGraph(drawPos.x - graphSize_.x, drawPos.y, lpImageMng.GetID(uiKey_)[0], true);
	}
}

void LockDoor::SetGraphSize(void)
{
	if (!isLock_ && firstTimeFlg_)
	{
		if (name_ == "White_A_Double")
		{
			uiKey_ = "UseKitchenUI";
		}
		else if (name_ == "Dark_B" || name_ == "Dark_B_Double")
		{
			uiKey_ = "UseMusicRoomUI";
		}
		else if (name_ == "Red_A")
		{
			uiKey_ = "UseFoodBankUI";
		}
		else if (name_ == "Dark_C")
		{
			uiKey_ = "UseChildRoomUI";
		}
		else if (name_ == "Red_C")
		{
			uiKey_ = "UseCellerUI";
		}
	}
	if (isLock_)
	{
		if (name_ == "White_A_Double")
		{
			uiKey_ = "KitchenUI";
		}
		else if (name_ == "Dark_B" || name_ == "Dark_B_Double")
		{
			uiKey_ = "MusicRoomUI";
		}
		else if (name_ == "Red_A")
		{
			uiKey_ = "FoodBankUI";
		}
		else if (name_ == "Dark_C")
		{
			uiKey_ = "ChildRoomUI";
		}
		else if (name_ == "Red_C")
		{
			uiKey_ = "CellerUI";
		}
		else if (name_ == "White_C_Double")
		{
			uiKey_ = "EntranceUI";
		}
	}
	GetGraphSize(lpImageMng.GetID(uiKey_)[0], &graphSize_.x, &graphSize_.y);
	graphSize_.x /= 2;
	graphSize_.y /= 2;
}

void LockDoor::VariableInit(void)
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
	//UI画像識別用
	uiKey_ = "";
	//経過時間
	delta_ = 0.0;
	//UI表示判定用フラグ
	uiFlg_ = false;
	//アニメーション種類
	animState_ = GIMMICK_ANIM::CLOSE;
	//ギミックタイプ
	type_ = GIMMICK_TYPE::DOOR;
	//初回確認用フラグ
	firstTimeFlg_ = true;
	//画像サイズの半分の値
	graphSize_ = { 0,0 };
	//描画位置
	drawPos = { lpSceneMng.GetScreenSize().x / 2 ,(lpSceneMng.GetScreenSize().y / 2) + OFFSET_Y };
	std::string directory = "resource/model/gimmick/";
	//使用するモデルを設定
	if (name_ == "White_A_Double")
	{
		modelStr_ = directory + "Door_White1_Double.mv1";
		//アニメーション追加
		animController_ = std::make_unique<AnimationController>(lpModelMng.GetID(modelStr_)[modelNum_]);
		animController_->Add((int)GIMMICK_ANIM::OPEN, directory +"anim/DoorDoubleOpen.mv1", OPEN_SPEED);
		animController_->Add((int)GIMMICK_ANIM::CLOSE, directory + "anim/DoorDoubleClose.mv1", CLOSE_SPEED);
	}
	else if (name_ == "Red_A")
	{
		modelStr_ = directory + "Door_Red1.mv1";
		//アニメーション追加
		animController_ = std::make_unique<AnimationController>(lpModelMng.GetID(modelStr_)[modelNum_]);
		animController_->Add((int)GIMMICK_ANIM::OPEN, directory + "anim/DoorOpen.mv1", OPEN_SPEED);
		animController_->Add((int)GIMMICK_ANIM::CLOSE, directory + "anim/DoorClose.mv1", CLOSE_SPEED);
	}
	else if (name_ == "Dark_B")
	{
		modelStr_ = directory + "Door_Dark2.mv1";
		//アニメーション追加
		animController_ = std::make_unique<AnimationController>(lpModelMng.GetID(modelStr_)[modelNum_]);
		animController_->Add((int)GIMMICK_ANIM::OPEN, directory + "anim/DoorOpen.mv1", OPEN_SPEED);
		animController_->Add((int)GIMMICK_ANIM::CLOSE, directory + "anim/DoorClose.mv1", CLOSE_SPEED);
	}
	else if (name_ == "Dark_B_Double")
	{
		modelStr_ = directory + "Door_Dark2_Double.mv1";
		//アニメーション追加
		animController_ = std::make_unique<AnimationController>(lpModelMng.GetID(modelStr_)[modelNum_]);
		animController_->Add((int)GIMMICK_ANIM::OPEN, directory + "anim/DoorDoubleOpen.mv1", OPEN_SPEED);
		animController_->Add((int)GIMMICK_ANIM::CLOSE, directory + "anim/DoorDoubleClose.mv1", CLOSE_SPEED);
	}
	else if (name_ == "Dark_C")
	{
		modelStr_ = directory + "Door_Dark3.mv1";
		//アニメーション追加
		animController_ = std::make_unique<AnimationController>(lpModelMng.GetID(modelStr_)[modelNum_]);
		animController_->Add((int)GIMMICK_ANIM::OPEN, directory + "anim/DoorOpen.mv1", OPEN_SPEED);
		animController_->Add((int)GIMMICK_ANIM::CLOSE, directory + "anim/DoorClose.mv1", CLOSE_SPEED);
	}
	else if (name_ == "Red_C")
	{
		modelStr_ = directory + "Door_Red3.mv1";
		//アニメーション追加
		animController_ = std::make_unique<AnimationController>(lpModelMng.GetID(modelStr_)[modelNum_]);
		animController_->Add((int)GIMMICK_ANIM::OPEN, directory + "anim/DoorOpen.mv1", OPEN_SPEED);
		animController_->Add((int)GIMMICK_ANIM::CLOSE, directory + "anim/DoorClose.mv1", CLOSE_SPEED);
	}
	else if(name_ == "White_C_Double")
	{
		modelStr_ = directory + "Door_White3_Double.mv1";
		//アニメーション追加
		animController_ = std::make_unique<AnimationController>(lpModelMng.GetID(modelStr_)[modelNum_]);
		animController_->Add((int)GIMMICK_ANIM::OPEN, directory + "anim/DoorDoubleOpen.mv1", OPEN_SPEED);
		animController_->Add((int)GIMMICK_ANIM::CLOSE, directory + "anim/DoorDoubleClose.mv1", CLOSE_SPEED);

	}

}
