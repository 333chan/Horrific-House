#include <DxLib.h>
#include"EffekseerForDXLib.h"
#include "PreTitleScene.h"
#include"TitleScene.h"
#include"GameScene.h"
#include"LoadScene.h"
#include "InventoryScene.h"
#include "BeginningScene.h"
#include "../Transition/FadeinOut.h"
#include "SceneManager.h"
#include "../common/SocketServer.h"
#include"../common/MouseIcon.h"
#include "../Common/Fps.h"
#include "../../resource/image/icon/icon.h"
#include "../../_debug/_DebugDispOut.h"
//画面サイズ
constexpr int SCREEN_SIZE_X = 1920;	//横
constexpr int SCREEN_SIZE_Y = 1080;	//縦

SceneManager::SceneManager()
{
	
}

SceneManager::~SceneManager()
{
	lpSocketServer.CommunicationCutoff();
}


void SceneManager::Run(void)
{
	if (!SysInit())
	{
		return;
	}
	//初期化
	Init();
	//ゲームループ
	while (ProcessMessage() == 0)
	{
		//デバッグ開始
		_dbgStartDraw();
		//更新
		Update();

		Draw();

		ScreenFlip();
		fps_->Wait();
	}
	//先にウィンドウを閉じる
	//SetWindowVisibleFlag(false);
	//effkseer終了
	Effkseer_End();
	//dxlib終了
	DxLib_End();
}

bool SceneManager::SysInit(void)
{
	SetGraphMode(static_cast<int>(SCREEN_SIZE_X), static_cast<int>(SCREEN_SIZE_Y), 32);
	ChangeWindowMode(false);
	//ChangeWindowMode(false);

	SetEnableXAudioFlag(true);
	// 64.0f で１メートルとする
	Set3DSoundOneMetre(1500.0f);
	SetWindowIconID(ICON_1);
	SetWindowText("Horrific House");
	if (DxLib_Init() == -1)
	{
		return false;
	}
#ifdef _DEBUG
	SetMouseDispFlag(true);
#else
	SetMouseDispFlag(false);
#endif
	_dbgSetup(static_cast<int>(SCREEN_SIZE_X), static_cast<int>(SCREEN_SIZE_Y), 255);
	return true;
}

void SceneManager::Init(void)
{
	//シーン初期化
	scene_ = std::make_unique<FadeinOut>(1.0f,Transition::In,std::make_unique<BeginningScene>());
	//
	sceneGraphFilter_ = MakeScreen(SCREEN_SIZE_X, SCREEN_SIZE_Y);
	brightnessScrren_ = 255;
	//デルタタイム初期化
	deltaTime_ = 0.0f;
	tickCount_ = std::chrono::system_clock::now();
	brightnessScrren_ = 255;
	//mouseIcon_ = std::make_unique<MouseIcon>();
	Init3D();
	//--Effekseer初期化--
	Effekseer_Init(8000);
	SetChangeScreenModeGraphicsSystemResetFlag(false);
	Effekseer_SetGraphicsDeviceLostCallbackFunctions();
	fps_ = std::make_unique<Fps>();
	//-------------------
	lpSocketServer.InternetConnection();
}

void SceneManager::Init3D(void)
{
	// Zバッファを有効にする
	SetUseZBuffer3D(true);

	// Zバッファへの書き込みを有効にする
	SetWriteZBuffer3D(true);

	// バックカリングを有効にする
	SetUseBackCulling(true);

	// クリップ距離を設定する
	//SetCameraNearFar(CAMERA_RANGE_NEAR, CAMERA_RANGE_FAR);

	//// ライトの設定
	//ChangeLightTypeDir({ 0.3f, -0.7f, 0.8f });

	// 背景色設定
	SetBackgroundColor(0, 0, 0);
	
}

void SceneManager::Update(void)
{
	lpSocketServer.Receive();
	UpdateMouse();

	auto tick = std::chrono::system_clock::now();
	deltaTime_ = std::chrono::duration_cast<std::chrono::microseconds>(tick - tickCount_).count() / 1000000.0f;
	tickCount_ = tick;

	fps_->Update();

	//更新
	scene_ = scene_->Update(std::move(scene_));
	lpMouseIcon.Update(scene_->GetSceneID());
}

void SceneManager::UpdateMouse()
{
	Vector2Int Mpos;
	GetMousePoint(&Mpos.x, &Mpos.y);
	if (scene_->GetSceneID() == SceneID::Game)
	{
		Mpos.x += lpSocketServer.GetCameraValue().first;
		Mpos.y -= lpSocketServer.GetCameraValue().second;
	}
	else
	{
		Mpos.x += lpSocketServer.GetCameraValue().first / 10;
		Mpos.y -= lpSocketServer.GetCameraValue().second / 10;
	}
	SetMousePoint(Mpos.x, Mpos.y);
}

void SceneManager::Draw(void)
{
	SetDrawScreen(sceneGraphFilter_);
	ClsDrawScreen();
	scene_->Draw();

	SetDrawScreen(DX_SCREEN_BACK);
	ClsDrawScreen();
	DrawGraph(0, 0, sceneGraphFilter_, true);

	lpMouseIcon.Draw();

	_dbgDraw();

}

Vector2Int SceneManager::GetScreenSize(void)
{
	return Vector2Int(SCREEN_SIZE_X, SCREEN_SIZE_Y);
}

double SceneManager::GetDeltaTime(void)
{
	return deltaTime_;
}

void SceneManager::SetBrightnessScrren(int brightnessScrren)
{
	brightnessScrren_ = brightnessScrren;
}

int SceneManager::GetBrightnessScrren()
{
	return brightnessScrren_;
}

void SceneManager::AddInventoryCnt()
{
	inventoryCnt_++;
}

int SceneManager::GetInventoryCnt()
{
	return inventoryCnt_;
}
