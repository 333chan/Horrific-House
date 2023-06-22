#include "OptionScene.h"
#include "TitleScene.h"
#include "GameScene.h"
#include "SceneExpansion/UI/OptionUI.h"
#include "SceneExpansion/buffer/GameCBuffer.h"
#include "SceneManager.h"
#include "../common/manager/ImageMng.h"
#include "../common/manager/CBufferMng.h"
#include "../common/MouseIcon.h"
#include "../common/CheckHit.h"
#include "../camera/Camera.h"
#include "ExitScene.h"
#include "../common/manager/ModelMng.h"
#include "../../_debug/_DebugDispOut.h"

OptionScene::OptionScene(UniqueScene ownScene, SceneID beforeID)
{
    beforeScene_ = std::move(ownScene);
    
    beforeSceneID_ = beforeID;

    Init();
    DrawScreen();
}

OptionScene::~OptionScene()
{
   
}

void OptionScene::Init(void)
{
    lpMouseIcon.Init();
    optionUI_ = std::make_unique<OptionUI>(beforeSceneID_); 
    
    //pos_ = { -1400,2000,-1300 };
    //カメラ作成
    camera_ = std::make_unique<Camera>();
    //カメラ処理(オブジェクト描画前に処理)
    camera_->SetBeforeDraw({ 0,0,0 });
    cBuffer_ = std::make_unique<GameCBuffer>();	//カメラ
}

UniqueScene OptionScene::Update(UniqueScene scene)
{
    optionUI_->Update();
    cBuffer_->Update(camera_->GetPos(), camera_->GetTargetPos(), Vector3{ 0.0f,0.0f,0.0f }, Vector3{ 0.0f,0.0f,0.0f }, {0,0,0});
   
    DrawScreen();
    return UpdateScene(scene);
}

void OptionScene::DrawScreen(void)
{
    SetDrawScreen(screenID_);
    ClsDrawScreen();
    DrawExtendGraph(0,
        0,
        lpSceneMng.GetScreenSize().x,
        lpSceneMng.GetScreenSize().y,
        lpImageMng.GetID("resource/image/option/bg1.png")[0],
        true);
    camera_->SetBeforeDraw({ 0,0,-5000 });

    //背景描画
    
   

    optionUI_->Draw();
#ifdef _DEBUG

#endif	//_DEBUG
   
}

void OptionScene::Release(void)
{
}

UniqueScene OptionScene::UpdateScene(UniqueScene& scene)
{
    switch (optionUI_->GetAfterSceneCol())
    {
    case OptionColUI::TitleReture:
        return std::make_unique<ExitScene>(GetSceneID(), std::move(beforeScene_), beforeSceneID_);
        break;
    case OptionColUI::Reture:
        return std::move(beforeScene_);
        break;
    default:
        break;
    }
    return  std::move(scene);
}


