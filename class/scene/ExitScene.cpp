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
    //�O��̃V�[�����擾
    sceneID_ = sceneID;
    //�I�v�V�����ɍs���Ƃ��ɃV�[���������Ă���
    beforeScene_ = std::move(ownScene);
    //�I�v�V�����ɍs���Ƃ���ID�������Ă���
    beforeSceneID_ = beforeID;
    //������
    Init();

    DrawScreen();
}

ExitScene::~ExitScene()
{
	Release();
}

void ExitScene::Init()
{
    //UI������
	exitUI_ = std::make_unique<ExitUI>(sceneID_);
}

UniqueScene ExitScene::Update(UniqueScene scene)
{
    //UI�X�V
	exitUI_->Update();
    //�`��
	DrawScreen();
	return UpdateScene(scene);
}

void ExitScene::DrawScreen(void)
{
	SetDrawScreen(screenID_);
	ClsDrawScreen();
    //UI�`��
	exitUI_->Draw();

}

void ExitScene::Release(void)
{
}

UniqueScene ExitScene::UpdateScene(UniqueScene& scene)
{
    //�I�������V�[���ɍs��
    switch (exitUI_->GetHitUICol())
    {
    case ExitColUI::Yes:
        //�g�тƂ̒ʐM��ؒf
        lpSocketServer.CommunicationCutoff();
        //effkseer�I��
        Effkseer_End();
        //dxlib�̏I��
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
    //�����Ȃ���Ό��̃V�[����Ԃ�
    return  std::move(scene);
}
