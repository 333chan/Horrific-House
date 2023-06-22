#include "PreTitleScene.h"
#include "SceneExpansion/UI/PreTitleUI.h"
#include "TitleScene.h"
#include "../Transition/FadeinOut.h"
#include "../common/manager/SoundMng.h"
PreTitleScene::PreTitleScene()
{
	Init();
	DrawScreen();
}

PreTitleScene::~PreTitleScene()
{
	Release();
}

void PreTitleScene::Init()
{
	//UI�̍쐬
	preTitleUI_ = std::make_unique<PreTitleUI>();
	lpSoundMng.ResetCnt("resource/sound/selectSE2.mp3");
	//PreLoading();
	Loading();
}

UniqueScene PreTitleScene::Update(UniqueScene scene)
{
	preTitleUI_->Update();
	if (preTitleUI_->GetPreEndFlag())
	{
		
		
		 return std::make_unique<FadeinOut>(1.0f, std::move(scene),std::make_unique<TitleScene>());
	}
	DrawScreen();
	return scene;
}

void PreTitleScene::DrawScreen(void)
{
	SetDrawScreen(screenID_);
	ClsDrawScreen();
	//DrawBox(0, 0, 100, 100, 0xff4455, true);
	preTitleUI_->Draw();
}

void PreTitleScene::Release(void)
{
}
