#include<DxLib.h>
#include "SceneExpansion/UI/ConnectedUI.h"
#include "ConnectedScene.h"
#include "LoadScene.h"
#include "../common/SocketServer.h"
#include "SelectScene.h"
#include "OpeningScene.h"
ConnectedScene::ConnectedScene()
{
    Init();
    DrawScreen();
}

ConnectedScene::~ConnectedScene()
{
    Release();
}

void ConnectedScene::Init()
{
    //UIの作成
    connectedUI_ = std::make_unique<ConnectedUI>();
}

UniqueScene ConnectedScene::Update(UniqueScene scene)
{
    //UIの更新
    connectedUI_->Update();
    //描画
    DrawScreen();
    return UpdateScene(scene);
}

void ConnectedScene::DrawScreen(void)
{
    SetDrawScreen(screenID_);
    ClsDrawScreen();
    //tmxcolの座標に画像を描画
    connectedUI_->Draw();
}

void ConnectedScene::Release(void)
{
}
UniqueScene ConnectedScene::UpdateScene(UniqueScene& scene)
{
    //選択したシーンに行く
    switch (connectedUI_->GetHitUICol())
    {
    case ConnectedColUI::Cancel:
        return std::make_unique<SelectScene>();
        break;
    default:
        break;
    }
    //携帯がつながったら、何も押さなくても次のシーンに行く
    if (lpSocketServer.GetIsConnection())
    {
        return std::make_unique<OpeningScene>(CntType::Smh);
    }
    //何もなければ元のシーンを返す
    return  std::move(scene);
}