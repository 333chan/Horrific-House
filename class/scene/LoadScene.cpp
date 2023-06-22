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
	//�I�������R���g���[���[�̃^�C�v
	cntType_ = type;
	//������
	Init();
	DrawScreen();
}
LoadScene::~LoadScene()
{
}

void LoadScene::Init(void)
{
	lpSoundMng.AllStopSound();
	//UI������
	loadUI_ = std::make_unique<LoadUI>();
}

UniqueScene LoadScene::Update(UniqueScene scene)
{
	loadUI_->Update();
	//���[�h���I������玟�̃V�[��
	if (loadUI_->GetIsLoadingEnd())
	{
		//�ネ�[�h
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
	//UI�`��
	loadUI_->Draw();
}

void LoadScene::Release(void)
{
}
