#include "LoadUI.h"
#include "../../SceneManager.h"
LoadUI::LoadUI()
{
	Init();
}

LoadUI::~LoadUI()
{
	// マスクデータを削除します
	DeleteMask(maskHandle_);
	DeleteMaskScreen();
}

void LoadUI::Init()
{
	LoadVol = 0;
	IsLoadingEnd = false;
	CreateMaskScreen();
	//マスク画像の読み込み
	maskHandle_ = LoadMask("resource/image/Load/gageImage3.png");
}

void LoadUI::Update()
{
	//仮で時間経過で勝手に上がっていく
	//TRACE("非同期読み込みの数:%d\n", GetASyncLoadNum());
	if (GetASyncLoadNum() == 0)//非同期読み込みの残数がいくつか調べる
	{
		//０なら
		//TRACE("非同期読み込み終了\n");
		if (LoadVol >= 1000)
		{
			IsLoadingEnd = true;//ロード終了
		}
		else//バーをマックスにする
		{
			LoadVol += 30;
			if (LoadVol > 1000)
			{
				LoadVol = 1000;
			}
		}
	}
	else
	{
		IsLoadingEnd = false;
		//ロードの進捗具合を表現してる
		if (LoadVol < 950)
		{
			LoadVol += 0.4f;

		}
	}
}

void LoadUI::Draw()
{
	//マスク描画	
	DrawMask(0, 0, maskHandle_, DX_MASKTRANS_NONE);
	//背景黒
	DrawBox(0, 0, 1920, 1080,
		0x000000,
		true);
	//ゲージの裏
	DrawGraph(0, 0, lpImageMng.GetID("fram")[0], true);
	//赤のゲージ
	DrawBox(0,
		0,
		lpSceneMng.GetScreenSize().x - 1200 + LoadVol,
		lpSceneMng.GetScreenSize().y,
		0x881100,
		true);
	
	
}

bool LoadUI::GetIsLoadingEnd()
{
	return IsLoadingEnd;
}
