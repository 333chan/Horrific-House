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

//モデル座標
constexpr float MODEL_POS_X = -1600.0f;
constexpr float MODEL_POS_Y =  2000.0f;
constexpr float MODEL_POS_Z =   790.0f;

TitleScene::TitleScene()
{
    //初期化
    Init();
    //ちらつき防止
    DrawScreen();
}

TitleScene::~TitleScene()
{
    Release();
}

void TitleScene::Init()
{
    //モデルのstring
    modleStr_ = "resource/model/stage/titlemap.mv1";
    //マップの座標
    pos_ = { MODEL_POS_X,MODEL_POS_Y,MODEL_POS_Z };
    //カメラ作成
    camera_     = std::make_unique<Camera>();
    //カメラ処理(オブジェクト描画前に処理)
    camera_->SetBeforeDraw(pos_);
    //定数バッファ作成
    cBuffer_    = std::make_unique<TitleCBuffer>(camera_->GetPos());
    //UIの作成
    titleUI_    = std::make_unique<TitleUI>();
    //シェーダークラス作成
    origShader_ = std::make_unique<OriginalShader>(
        LoadVertexShader("resource/shader/vs/NormalModel_vs.vso"),
        LoadPixelShader("resource/shader/ps/TitleModel_ps.pso"));
    //座標セット
    MV1SetPosition(lpModelMng.GetID(modleStr_)[0], {0,0,0 });
    
    //音を全て止める
    lpSoundMng.AllStopSound();    
    lpSoundMng.PlaySoundOneTime("resource/sound/selectSE2.mp3");
    lpSoundMng.ResetCnt("resource/sound/cursorSE.mp3");
    //BGMを流す
    PlaySoundMem(lpSoundMng.GetID("./resource/sound/TitleBGM.mp3")[0], DX_PLAYTYPE_LOOP);
}

UniqueScene TitleScene::Update(UniqueScene scene)
{
    //Cバッファの更新
    cBuffer_->Update();
    //タイトルモデルの描画
    titleUI_->Update();
    
    DrawScreen();
    return UpdateScene(scene);
}

void TitleScene::DrawScreen(void)
{
    SetDrawScreen(screenID_);
    ClsDrawScreen();

    //カメラ処理(オブジェクト描画前に処理)
    camera_->SetBeforeDraw(pos_);
    //モデルの描画
    origShader_->Draw(lpModelMng.GetID(modleStr_)[0]);

    //UIを描画
    titleUI_->Draw();

    //デバッグ用
#ifdef _DEBUG
#endif	//_DEBUG
}

void TitleScene::Release(void)
{
}

UniqueScene TitleScene::UpdateScene(UniqueScene& scene)
{
    //選択したシーンに移動
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
    //何もなければ元のシーンを返す
    return  std::move(scene);
}


