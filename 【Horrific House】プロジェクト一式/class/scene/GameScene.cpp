#include<DxLib.h>
#include<EffekseerForDXLib.h>
#include "../common/manager/CBufferMng.h"
#include "../common/manager/ImageMng.h"
#include "../common/manager/SoundMng.h"
#include "../common/OriginalShader.h"
#include "../common/manager/ModelMng.h"
#include "SceneManager.h"

#include "../camera/Camera.h"
#include "../input/KeyInput.h"
#include "../input/PadInput.h"

#include "SceneExpansion/buffer/GameCBuffer.h"
#include "SceneExpansion/UI/GameUI.h"
#include "GameScene.h"
#include "ResultScene.h"
#include "InventoryScene.h"
#include "GameOverScene.h"
#include "OptionScene.h"
#include "../object/player/Player.h"
#include "../object/enemy/Enemy.h"
#include "../object/Stage.h"

#include "../object/Goal.h"
#include"../object/Item/Item.h"

#include "../../_debug/_DebugConOut.h"
#include "EndingScene.h"
#include "../Transition/FadeinOut.h"
#include "../common/EffekseerEffectHandle.hpp"
#include "../scene/Effect/Effects.h"

constexpr int RANGE = 2000;
constexpr float DEF_SENS = 15.0f; //通常感度
constexpr float LOW_SENS = 30.0f; //低感度

constexpr float EFFECT_SCALE = 70.0f;//埃のエフェクトの大きさ
//constexpr float EFFECT_PLAY_SPEED = 1.0f;//埃のエフェクトの再生速度
constexpr float GOAL_POS_X = 1300.0f;
constexpr float GOAL_POS_Y = 1350.0f;
constexpr float GOAL_POS_Z = 2500.0f;

constexpr float CAMERA_ANGLE = 180.0f;

GameScene::GameScene()
{
	cntType_ = CntType::Key;
	Init();
}
GameScene::GameScene(CntType type)
{
	cntType_ = type;
	Init();
	gameUI_ = std::make_unique<GameUI>(type);
}

GameScene::~GameScene()
{
	Release();
}

void GameScene::Init(void)
{
	
	if (cntType_ == CntType::Smh)
	{
		//スマホだったら、キーボード入力にする
		controller_ = std::make_unique<KeyInput>();
	}
	else
	{
		//パッドの数が１つ以上の場合はパッド操作にする
		if (GetJoypadNum() > 0)
		{
			controller_ = std::make_unique<PadInput>();
		}
		else
		{
			controller_ = std::make_unique<KeyInput>();
		}
	}

	//グラフィックサイズを確保
	inventoryBackGroundHandle_ = MakeGraph(lpSceneMng.GetScreenSize().x, lpSceneMng.GetScreenSize().y);

	//スクリーンを作成
	screenCameraID_ = MakeScreen(screenSizeX_, screenSizeY_);
	GaussScreenCameraID_ = MakeScreen(screenSizeX_, screenSizeY_);
	DepthMapTextureID_ = MakeScreen(screenSizeX_, screenSizeY_);
	//インスタンス生成
	stage_  = std::make_unique<Stage>();																					//ステージ
	player_ = std::make_unique<Player>(controller_->GetControllerType(), stage_->GetGimmickList(), stage_->GetItemList());	//プレイヤー
	camera_ = std::make_unique<Camera>(Vector3(0.0f,Deg2RadF(CAMERA_ANGLE),0.0f));												//カメラ
	cBuffer_= std::make_unique<GameCBuffer>(defPos_ = player_->GetPosition(), defTargetPos_ = camera_->GetTargetPos());		//定数バッファ
	enemy_  = std::make_unique<Enemy>(stage_->GetGimmickList());															//敵
	goal_   = std::make_unique<Goal>("resource/model/Goal.mv1", Vector3{ GOAL_POS_X, GOAL_POS_Y, GOAL_POS_Z });								//ゴール
	//シェーダー
	origShader_ = std::make_unique<OriginalShader>(LoadPixelShader("resource/shader/ps/DfferedLighting_ps.pso"));
	int VertexShader = LoadVertexShader("resource/shader/vs/NormalModel_vs.vso");
	int depthPixelShader = LoadPixelShader("resource/shader/ps/Depth_ps.pso");
	depthShader_ = std::make_unique<OriginalShader>(VertexShader, depthPixelShader);
	//音
	lpSoundMng.AllStopSound();
	PlaySoundMem(lpSoundMng.GetID("./resource/sound/gameBGM.mp3")[0], DX_PLAYTYPE_LOOP);

	lpSoundMng.SetDefaultVolume();

	//ポストエフェクト
	psPostEffect_ = LoadPixelShader("./resource/shader/ps/HuntedPlayer_ps.pso");
	postEffect_ = std::make_unique<OriginalShader>(psPostEffect_);
	//埃エフェクトの初期化
	dust_.resource = LoadEffekseerEffect("resource/effect/dust.efkefc", EFFECT_SCALE);
	dust_.playhandle = -1;
	dust_.playhandle = PlayEffekseer3DEffect(dust_.resource);
	dust_.pos = player_->GetPosition();
	SetPosPlayingEffekseer3DEffect(dust_.playhandle, dust_.pos.x, dust_.pos.y, dust_.pos.z);

}

UniqueScene GameScene::Update(UniqueScene scene)
{
	UpdateEffekseer3D();
	//コントローラ
	controller_->Update();
	enemy_->GetTargetLine();
	cBuffer_->Update(camera_->GetPos(), camera_->GetTargetPos(), enemy_->GetPosition(), enemy_->GetDirectionVec(), enemy_->GetTargetLine());
	//UIの更新
	gameUI_->Update();
	//ステージ
	stage_->Update();
	//カメラ
	camera_->Update(player_->GetDashFlag());
	//プレイヤー
	player_->GetItemID();
	player_->Update(camera_->GetAngles(), camera_->GetTargetPos());
	//敵
	enemy_->Update(player_->GetCapsule(), player_->GetPosition(), player_->CheckHitFlash(camera_->GetTargetPos(),camera_->GetAngles()),player_->GetItemID(),stage_->GetGimmickList());
	//低感度状態がtureだったら
	if (player_->GetLowSensFlg())
	{
		//低感度
		camera_->SetSensitivity(LOW_SENS);
	}
	else
	{
		//通常感度
		camera_->SetSensitivity(DEF_SENS);
	}
	
	if (abs(dust_.pos.x - camera_->GetPos().x) > RANGE ||
		abs(dust_.pos.z - camera_->GetPos().z) > RANGE)
	{
		//範囲外に入ったら埃エフェクトを更新
		StopEffekseer3DEffect(dust_.playhandle);
		dust_.playhandle = PlayEffekseer3DEffect(dust_.resource);
		dust_.pos = camera_->GetPos();
		SetPosPlayingEffekseer3DEffect(dust_.playhandle, dust_.pos.x, dust_.pos.y, dust_.pos.z);
	}


	//オプション
	if (controller_->CheckInputKey(KeyID::Pause))
	{
		StopSoundMem(lpSoundMng.GetID("./resource/sound/pianoBGM2.mp3")[0]);

		return std::make_unique<OptionScene>(std::move(scene),GetSceneID());
	}
	//インベントリ
	if (controller_->CheckInputKey(KeyID::Inventory))
	{
		lpSoundMng.PlayingSound("resource/sound/InventoryOpenSE.mp3");
		//初回インベントリ展開時、Tutorialflgをtrue
		lpTutorialMng.SetTutorialFlg(TutorialProgress::Inventry, true);
		//表示画面をインベントリで背景にする
		lpSceneMng.AddInventoryCnt();
		GetDrawScreenGraph(0, 0, lpSceneMng.GetScreenSize().x, lpSceneMng.GetScreenSize().y, inventoryBackGroundHandle_);
		StopSoundMem(lpSoundMng.GetID("./resource/sound/pianoBGM2.mp3")[0]);

		return std::make_unique<InventoryScene>(std::move(scene), inventoryBackGroundHandle_, controller_->GetControllerType(), player_->GetInventoryList(), player_->GetMinCameraPhotoVector());
	}
	//ゲームオーバー時
	if (enemy_->GetHitPlayerFlag())
	{
		StopSoundMem(lpSoundMng.GetID("./resource/sound/pianoBGM2.mp3")[0]);
		return std::make_unique<GameOverScene>(controller_->GetControllerType(), player_->GetPosition(), enemy_->GetPosition(), camera_->GetAngles(), cntType_);

	}
	//ゲームクリア時
	if (player_->GetIsGameClear())
	{
		StopSoundMem(lpSoundMng.GetID("./resource/sound/pianoBGM2.mp3")[0]);
		return std::make_unique<FadeinOut>(1.0f, std::move(scene),std::make_unique<EndingScene>(cntType_));
	}

	//デバッグ用
#ifdef _DEBUG
	if (CheckHitKey(KEY_INPUT_R))
	{
		return std::make_unique<ResultScene>(controller_->GetControllerType());
	}
#endif 

	//描画
	DrawScreen();

	return scene;
}

void GameScene::DrawScreen(void)
{
	SetDrawScreen(screenCameraID_);
	ClsDrawScreen();

	//カメラ処理(オブジェクト描画前に処理)
	camera_->SetBeforeDraw(player_->GetPosition());
	//ステージ
	stage_->Draw(player_->GetminCameraFlag(),depthRT_);
	//敵
	enemy_->Draw();
	//プレイヤー
	player_->SetMinCameraRenderTarget(screenCameraID_);

	SetDrawScreen(screenID_);
	ClsDrawScreen();
	
	if (enemy_->GetHuntFlag())
	{
		postEffect_->DrawOnlyPixel(0.0f, 0.0f, screenCameraID_, -1, -1, -1);
	}
	else
	{
		DrawGraph(0, 0, screenCameraID_, true);
	}

	DrawEffekseer3D();
	//プレイヤーの描画
	player_->Draw();
	//UIの描画
	gameUI_->Draw();

#ifdef _DEBUG
	
#endif
}

void GameScene::Release(void)
{
	DeleteGraph(inventoryBackGroundHandle_);
}
