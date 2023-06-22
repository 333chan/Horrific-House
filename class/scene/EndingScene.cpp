#include "../common/manager/SoundMng.h"
#include "../common/manager/ModelMng.h"
#include "SceneExpansion/UI/EndingUI.h"
#include "EndingScene.h"
#include "ResultScene.h"
#include "../Transition/FadeinOut.h"
#include "..\input\KeyInput.h"
#include "..\input\PadInput.h"


EndingScene::EndingScene()
{
    Init();
    DrawScreen();
}

EndingScene::EndingScene(CntType type)
{
    cntType_ = type;

    switch (cntType_)
    {
    case CntType::Non:
        break;
    case CntType::Key:
        controller_ = std::make_unique<KeyInput>();
        break;
    case CntType::Pad:
        controller_ = std::make_unique<PadInput>();
        break;
    case CntType::Smh:
        if (GetJoypadNum() > 0)
        {
            controller_ = std::make_unique<PadInput>();
        }
        else
        {
            controller_ = std::make_unique<KeyInput>();
        }
        break;
    case CntType::Max:
        break;
    default:
        break;
    }
    Init();
    DrawScreen();
}

EndingScene::~EndingScene()
{
}

void EndingScene::Init()
{
    endingUI_ = std::make_unique<EndingUI>(cntType_);
    lpSoundMng.ChangeVolume(200, "./resource/sound/run_Grass.mp3");
}

UniqueScene EndingScene::Update(UniqueScene scene)
{
    endingUI_->Update();
    UpdateSE();

    //オープニングが終わったら
    if (endingUI_->GetIsEndFlg())
    {
        return std::make_unique<FadeinOut>(1.0f, std::move(scene), std::make_unique<ResultScene>(controller_->GetControllerType()));
    }
    DrawScreen();
    return scene;
}

void EndingScene::DrawScreen(void)
{
    SetDrawScreen(screenID_);
    ClsDrawScreen();
    endingUI_->Draw();
}

void EndingScene::Release(void)
{
}

void EndingScene::UpdateSE()
{
    auto progress = endingUI_->Getprogress();
    switch (progress)
    {
    case (int)EndingColUI::Non:
        lpSoundMng.PlayingSound("resource/sound/PlayerRunVoice.mp3");
        lpSoundMng.PlayingSound("resource/sound/run_Grass.mp3");
        lpSoundMng.ResetCnt("resource/sound/PlayerRunOutVoice.mp3");
        break;
    case (int)EndingColUI::First:
        lpSoundMng.StopSound("resource/sound/PlayerRunVoice.mp3");
        
        lpSoundMng.PlaySoundOneTime("resource/sound/PlayerRunOutVoice.mp3");
        lpSoundMng.PlayingSound("resource/sound/seaBGM.mp3");
        lpSoundMng.ChangeVolume(50,"resource/sound/seaBGM.mp3");
        lpSoundMng.ChangeVolume(150,"resource/sound/run_Grass.mp3");
        break;
    case (int)EndingColUI::Second:
       
        lpSoundMng.StopSound("resource/sound/PlayerRunOutVoice.mp3");
        lpSoundMng.ChangeVolume(100, "resource/sound/seaBGM.mp3");
        lpSoundMng.ChangeVolume(120, "resource/sound/run_Grass.mp3");
        break;
    case (int)EndingColUI::Third:
        lpSoundMng.StopSound("resource/sound/run_Grass.mp3");
        lpSoundMng.ChangeVolume(170, "resource/sound/seaBGM.mp3");
        break;
    case (int)EndingColUI::Forth:
        lpSoundMng.ResetCnt("resource/sound/guuuSE.mp3");
        break;
    case (int)EndingColUI::Five:
        lpSoundMng.PlaySoundOneTime("resource/sound/guuuSE.mp3");
        break;
    case (int)EndingColUI::Six:
        lpSoundMng.ChangeVolume(150, "resource/sound/seaBGM.mp3");
        break;
    default:
        break;
    }

}

UniqueScene EndingScene::UpdateScene(UniqueScene& scene)
{
    return  std::move(scene);
}