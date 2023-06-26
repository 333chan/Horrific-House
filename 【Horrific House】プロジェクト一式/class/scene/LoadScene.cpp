#include "../common/manager/ModelMng.h"
#include "LoadScene.h"
#include "GameScene.h"
#include "SceneExpansion/UI/LoadUI.h"
#include "../common/manager/SoundMng.h"
LoadScene::LoadScene()
{
	Init();
	DrawScreen();
}

LoadScene::LoadScene(CntType type)
{
	//選択したコントローラーのタイプ
	cntType_ = type;
	//初期化
	Init();
	DrawScreen();
}
LoadScene::~LoadScene()
{
}

void LoadScene::Init(void)
{
	lpSoundMng.AllStopSound();
	//UI初期化
	loadUI_ = std::make_unique<LoadUI>();
}

UniqueScene LoadScene::Update(UniqueScene scene)
{
	loadUI_->Update();
	//ロードが終わったら次のシーン
	if (loadUI_->GetIsLoadingEnd())
	{
		//後ロード
		AfterLoading();
		return std::move(std::make_unique<GameScene>(cntType_));
	}
	DrawScreen();
	return scene;
}

void LoadScene::DrawScreen(void)
{
	SetDrawScreen(screenID_);
	ClsDrawScreen();
	//UI描画
	loadUI_->Draw();
}

void LoadScene::Release(void)
{
}
