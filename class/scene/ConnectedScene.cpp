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
    //UI�̍쐬
    connectedUI_ = std::make_unique<ConnectedUI>();
}

UniqueScene ConnectedScene::Update(UniqueScene scene)
{
    //UI�̍X�V
    connectedUI_->Update();
    //�`��
    DrawScreen();
    return UpdateScene(scene);
}

void ConnectedScene::DrawScreen(void)
{
    SetDrawScreen(screenID_);
    ClsDrawScreen();
    //tmxcol�̍��W�ɉ摜��`��
    connectedUI_->Draw();
}

void ConnectedScene::Release(void)
{
}
UniqueScene ConnectedScene::UpdateScene(UniqueScene& scene)
{
    //�I�������V�[���ɍs��
    switch (connectedUI_->GetHitUICol())
    {
    case ConnectedColUI::Cancel:
        return std::make_unique<SelectScene>();
        break;
    default:
        break;
    }
    //�g�т��Ȃ�������A���������Ȃ��Ă����̃V�[���ɍs��
    if (lpSocketServer.GetIsConnection())
    {
        return std::make_unique<OpeningScene>(CntType::Smh);
    }
    //�����Ȃ���Ό��̃V�[����Ԃ�
    return  std::move(scene);
}