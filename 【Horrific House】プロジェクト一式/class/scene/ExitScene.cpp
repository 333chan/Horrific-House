#include "SceneManager.h"
#include "../common/manager/ImageMng.h"
#include "../common/SocketServer.h"
#include "ExitScene.h"
#include "TitleScene.h"
#include "OptionScene.h"
#include "SceneExpansion/UI/ExitUI.h"


ExitScene::ExitScene()
{
	Init();

	DrawScreen();
}

ExitScene::ExitScene(SceneID sceneID)
{
    sceneID_ = sceneID;
    Init();

    DrawScreen();
}

ExitScene::ExitScene(SceneID sceneID, UniqueScene ownScene, SceneID beforeID)
{
    //前回のシーンを取得
    sceneID_ = sceneID;
    //オプションに行くときにシーンを持っておく
    beforeScene_ = std::move(ownScene);
    //オプションに行くときにIDを持っておく
    beforeSceneID_ = beforeID;
    //初期化
    Init();

    DrawScreen();
}

ExitScene::~ExitScene()
{
	Release();
}

void ExitScene::Init()
{
    //UI初期化
	exitUI_ = std::make_unique<ExitUI>(sceneID_);
}

UniqueScene ExitScene::Update(UniqueScene scene)
{
    //UI更新
	exitUI_->Update();
    //描画
	DrawScreen();
	return UpdateScene(scene);
}

void ExitScene::DrawScreen(void)
{
	SetDrawScreen(screenID_);
	ClsDrawScreen();
    //UI描画
	exitUI_->Draw();

}

void ExitScene::Release(void)
{
}

UniqueScene ExitScene::UpdateScene(UniqueScene& scene)
{
    //選択したシーンに行く
    switch (exitUI_->GetHitUICol())
    {
    case ExitColUI::Yes:
        //携帯との通信を切断
        lpSocketServer.CommunicationCutoff();
        //effkseer終了
        Effkseer_End();
        //dxlibの終了
        DxLib_End();
        break;
    case ExitColUI::YesOption:
        return std::make_unique<TitleScene>();
        break;
    case ExitColUI::No:
        return std::make_unique<TitleScene>();
        break;
    case ExitColUI::NoOption:
        return std::make_unique<OptionScene>(std::move(beforeScene_), beforeSceneID_);
        break;
    default:
        break;
    }
    //何もなければ元のシーンを返す
    return  std::move(scene);
}
