
#include "../common/manager/SoundMng.h"
#include "../common/manager/ModelMng.h"
#include "SceneExpansion/UI/OpeningUI.h"
#include "OpeningScene.h"
#include "LoadScene.h"
#include "../Transition/FadeinOut.h"


OpeningScene::OpeningScene()
{
    Init();
    DrawScreen();
}

OpeningScene::OpeningScene(CntType type)
{
    cntType_ = type;
    Init();
    DrawScreen();
}

OpeningScene::~OpeningScene()
{
}

void OpeningScene::Init()
{
    
    openingUI_ = std::make_unique<OpeningUI>(cntType_);
    
}

UniqueScene OpeningScene::Update(UniqueScene scene)
{
    openingUI_->Update();
    UpdateSE();

    //オープニングが終わったら
    if (openingUI_->GetIsEndFlg())
    {
        return std::make_unique<FadeinOut>(1.0f, std::move(scene), std::make_unique<LoadScene>(cntType_));
    }
    DrawScreen();
    return scene;
}

void OpeningScene::DrawScreen(void)
{
    SetDrawScreen(screenID_);
    ClsDrawScreen();
    openingUI_->Draw();
}

void OpeningScene::Release(void)
{
}

void OpeningScene::UpdateSE()
{
   

    auto progress = openingUI_->Getprogress();
    switch (progress)
    {
    case (int)OpeningColUI::First:
        lpSoundMng.AllStopSound();
        lpSoundMng.PlayingSound("resource/sound/opbgm.mp3");
        lpSoundMng.ResetCnt("resource/sound/DoorOpenSE.mp3");
        break;
    case (int)OpeningColUI::Third:
        lpSoundMng.PlaySoundOneTime("resource/sound/DoorOpenSE.mp3");
        lpSoundMng.ResetCnt("resource/sound/DoorSE.mp3");
        break;
    case (int)OpeningColUI::Forth:
        lpSoundMng.PlaySoundOneTime("resource/sound/DoorSE.mp3");
        lpSoundMng.ResetCnt("resource/sound/DoorCloseSE.mp3");
        break;
    
    case (int)OpeningColUI::Five:
        lpSoundMng.PlaySoundOneTime("resource/sound/DoorCloseSE.mp3");
        lpSoundMng.ResetCnt("resource/sound/LockDoorSE.mp3");
        lpSoundMng.ChangeVolume(150,"resource/sound/opbgm.mp3");
        break;
    case (int)OpeningColUI::Six:
        lpSoundMng.PlaySoundOneTime("resource/sound/LockDoorSE.mp3");
        break;
    default:
        break;
    }
 
}