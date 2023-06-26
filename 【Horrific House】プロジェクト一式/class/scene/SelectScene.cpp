#include<DxLib.h>
#include "OpeningScene.h"
#include "SelectScene.h"
#include "LoadScene.h"
#include "TitleScene.h"
#include "ConnectedScene.h"
#include "SceneExpansion/UI/SelectUI.h"
#include "../common/manager/SoundMng.h"

SelectScene::SelectScene()
{
    Init();
    DrawScreen();
}

SelectScene::~SelectScene()
{
    Release();
}

void SelectScene::Init()
{
    lpSoundMng.ResetCnt("resource/sound/gamestartSE.mp3");
    //UI�̍쐬
    selectUI_ = std::make_unique<SelectUI>();
}

UniqueScene SelectScene::Update(UniqueScene scene)
{
    //UI�X�V
    selectUI_->Update();
    DrawScreen();
    return UpdateScene(scene);
}

void SelectScene::DrawScreen(void)
{
    SetDrawScreen(screenID_);
    ClsDrawScreen();
    //tmxcol�̍��W�ɉ摜��`��
    selectUI_->Draw();
}

void SelectScene::Release(void)
{
}
UniqueScene SelectScene::UpdateScene(UniqueScene& scene)
{
    //�I�������V�[��
    switch (selectUI_->GetHitUICol())
    {
    case SelectColUI::Decide:
        if (selectUI_->GetCntType() == CntType::Smh)
        {
            return std::make_unique<ConnectedScene>();
        }
        else
        {
            lpSoundMng.AllStopSound();
            lpSoundMng.PlayingSound("resource/sound/gamestartSE.mp3");
            return std::make_unique<OpeningScene>(selectUI_->GetCntType());
        }
        break;
    case SelectColUI::Return:
        return std::make_unique<TitleScene>();
        break;
    default:
        break;
    }
    //�����Ȃ���Ό��̃V�[����Ԃ�
    return  std::move(scene);
}

