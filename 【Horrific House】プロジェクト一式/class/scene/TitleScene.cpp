#include<DxLib.h>
#include<string>
#include "SceneManager.h"
#include "../common/manager/SoundMng.h"
#include "../common/manager/ModelMng.h"
#include "../common/OriginalShader.h"
#include "../camera/Camera.h"

#include "TitleScene.h"
#include "OptionScene.h"
#include "SelectScene.h"
#include "ExitScene.h"
#include "SceneExpansion/buffer/TitleCBuffer.h"
#include "SceneExpansion/UI/TitleUI.h"

#include "../../_debug/_DebugConOut.h"

//���f�����W
constexpr float MODEL_POS_X = -1600.0f;
constexpr float MODEL_POS_Y =  2000.0f;
constexpr float MODEL_POS_Z =   790.0f;

TitleScene::TitleScene()
{
    //������
    Init();
    //������h�~
    DrawScreen();
}

TitleScene::~TitleScene()
{
    Release();
}

void TitleScene::Init()
{
    //���f����string
    modleStr_ = "resource/model/stage/titlemap.mv1";
    //�}�b�v�̍��W
    pos_ = { MODEL_POS_X,MODEL_POS_Y,MODEL_POS_Z };
    //�J�����쐬
    camera_     = std::make_unique<Camera>();
    //�J��������(�I�u�W�F�N�g�`��O�ɏ���)
    camera_->SetBeforeDraw(pos_);
    //�萔�o�b�t�@�쐬
    cBuffer_    = std::make_unique<TitleCBuffer>(camera_->GetPos());
    //UI�̍쐬
    titleUI_    = std::make_unique<TitleUI>();
    //�V�F�[�_�[�N���X�쐬
    origShader_ = std::make_unique<OriginalShader>(
        LoadVertexShader("resource/shader/vs/NormalModel_vs.vso"),
        LoadPixelShader("resource/shader/ps/TitleModel_ps.pso"));
    //���W�Z�b�g
    MV1SetPosition(lpModelMng.GetID(modleStr_)[0], {0,0,0 });
    
    //����S�Ď~�߂�
    lpSoundMng.AllStopSound();    
    lpSoundMng.PlaySoundOneTime("resource/sound/selectSE2.mp3");
    lpSoundMng.ResetCnt("resource/sound/cursorSE.mp3");
    //BGM�𗬂�
    PlaySoundMem(lpSoundMng.GetID("./resource/sound/TitleBGM.mp3")[0], DX_PLAYTYPE_LOOP);
}

UniqueScene TitleScene::Update(UniqueScene scene)
{
    //C�o�b�t�@�̍X�V
    cBuffer_->Update();
    //�^�C�g�����f���̕`��
    titleUI_->Update();
    
    DrawScreen();
    return UpdateScene(scene);
}

void TitleScene::DrawScreen(void)
{
    SetDrawScreen(screenID_);
    ClsDrawScreen();

    //�J��������(�I�u�W�F�N�g�`��O�ɏ���)
    camera_->SetBeforeDraw(pos_);
    //���f���̕`��
    origShader_->Draw(lpModelMng.GetID(modleStr_)[0]);

    //UI��`��
    titleUI_->Draw();

    //�f�o�b�O�p
#ifdef _DEBUG
#endif	//_DEBUG
}

void TitleScene::Release(void)
{
}

UniqueScene TitleScene::UpdateScene(UniqueScene& scene)
{
    //�I�������V�[���Ɉړ�
    switch (titleUI_->GetHitUICol())
    {
    case TitleColUI::Play:
        lpSoundMng.ResetCnt("resource/sound/cursorSE.mp3");
        lpSoundMng.PlaySoundOneTime("resource/sound/cursorSE.mp3");

        return std::make_unique<SelectScene>();
        break;
    case TitleColUI::Option:
        lpSoundMng.ResetCnt("resource/sound/cursorSE.mp3");
        lpSoundMng.PlaySoundOneTime("resource/sound/cursorSE.mp3");

        return std::make_unique<OptionScene>(std::move(scene),GetSceneID());
        break;
    case TitleColUI::Exit:
        lpSoundMng.ResetCnt("resource/sound/cursorSE.mp3");
        lpSoundMng.PlaySoundOneTime("resource/sound/cursorSE.mp3");

        return std::make_unique<ExitScene>(GetSceneID());
        break;
    default:
        break;
    }
    //�����Ȃ���Ό��̃V�[����Ԃ�
    return  std::move(scene);
}


