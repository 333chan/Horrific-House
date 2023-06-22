#include<DxLib.h>
#include "ResultScene.h"
#include "../common/manager/ImageMng.h"
#include "../common/manager/ModelMng.h"
#include "SceneManager.h"
#include "TitleScene.h"
#include"SceneExpansion/buffer/ResultCBuffer.h"
#include "../input/PadInput.h"
#include "../input/KeyInput.h"
#include "../tmx/TmxObj.h"
#include "SceneExpansion/UI/ResultUI.h"
#include"../camera/Camera.h"
#include"../common/OriginalShader.h"
#include "../../_debug/_DebugConOut.h"

//初期位置
constexpr VECTOR CAMERA_INIT_POS = { 0, 400, -400 };                    //カメラ
constexpr VECTOR CLIFF_INIT_POS = { -470.0f, -440.0f, -2270.0f };       //崖
constexpr VECTOR WATER_INIT_POS = { 0, 200, 0 };                        //水
constexpr VECTOR TAREGET_INIT_POS = { 0, 400, 400 };                    //注視点
constexpr VECTOR GRASS_INIT_POS = { 8, 175, 0 };                        //草
constexpr float ADD_ROTATE = 0.0001f;                                   //回転加算値

//キューブテクスチャ
constexpr int CUBE_TEXTURE_NUM = 6; //数
constexpr int CUBE_SIZE = 512;      //テクスチャサイズ

constexpr float SKYDOME_SCALE = 30.0f;  //スカイドームの大きさ
constexpr float GRASS_SCALE = 0.4f;

ResultScene::ResultScene(ControllerType type)
{
    contorollerType_ = type;
    Init();
    DrawScreen();
}

ResultScene::~ResultScene()
{
    Release();
}

void ResultScene::Init(void)
{
    //大きさや位置初期化
    //スカイドーム
    MV1SetScale(lpModelMng.GetID("./resource/model/stage/dome/Skydome.mv1")[0], VGet(SKYDOME_SCALE, SKYDOME_SCALE, SKYDOME_SCALE));
    //水
    MV1SetPosition(lpModelMng.GetID("./resource/model/stage/water100.mv1")[0], WATER_INIT_POS);
    //足場
    MV1SetPosition(lpModelMng.GetID("./resource/model/stage/cliff.mv1")[0], CLIFF_INIT_POS);
    //草
    MV1SetPosition(lpModelMng.GetID("./resource/model/stage/grass.mv1")[0], GRASS_INIT_POS);
    MV1SetScale(lpModelMng.GetID("./resource/model/stage/grass.mv1")[0], VGet(GRASS_SCALE, GRASS_SCALE, GRASS_SCALE));

    //コントローラ設定
    if (contorollerType_ == ControllerType::Pad)
    {
        controller_ = std::make_unique<PadInput>();
    }
    else
    {
        controller_ = std::make_unique<KeyInput>();
    }

    //UI
    resultUI_ = std::make_unique<ResultUI>();
    //カメラ
    camera_ = std::make_unique<Camera>(Vector3(0.0f, Deg2RadF(180.0f), 0.0f));
    //定数バッファ
    cBuffer_ = std::make_unique<ResultCBuffer>();

    //シェーダ
    origShader_ = std::make_unique<OriginalShader>(LoadVertexShader("./resource/shader/vs/Sea_vs.vso"), LoadPixelShader("./resource/shader/ps/Sea_ps.pso"));

    //カウント
    stepCount_ = 0.0f;
    //スカイドームの回転
    skyRotate_ = 0.0f;

    //キューブテクスチャとする
    SetCubeMapTextureCreateFlag(true);
    cubeTexture_ = MakeScreen(CUBE_SIZE, CUBE_SIZE, true);
    SetCubeMapTextureCreateFlag(false);

    //SkyDomeをY軸反転させる
    MV1SetRotationXYZ(lpModelMng.GetID("./resource/model/stage/dome/Skydome.mv1")[0], VGet(Deg2RadF(180.0f), skyRotate_, 0.0f));
    CreateCubeMap();
    MV1SetPosition(lpModelMng.GetID("./resource/model/stage/grass.mv1")[0], GRASS_INIT_POS);
}

UniqueScene ResultScene::Update(UniqueScene scene)
{
    //コントローラ
    controller_->Update();
    //UI
    resultUI_->Update();
    //定数バッファ
    cBuffer_->Update(stepCount_, Vector3(0.0f, 0.0f, 0.0f));

    //描画
    DrawScreen();

    //毎フレーム加算
    stepCount_ += lpSceneMng.GetDeltaTime();
    skyRotate_ += ADD_ROTATE;

    return UpdateScene(scene);
}

void ResultScene::DrawScreen(void)
{
    //先にキューブマップに描画する
    CreateCubeMap();

    SetDrawScreen(screenID_);
    ClsDrawScreen();

    //カメラ処理(オブジェクト描画前に処理)
    SetCameraPositionAndTarget_UpVecY(CAMERA_INIT_POS, TAREGET_INIT_POS);

    //スカイドーム
    MV1DrawModel(lpModelMng.GetID("./resource/model/stage/dome/Skydome.mv1")[0]);
    //足場
    MV1DrawModel(lpModelMng.GetID("./resource/model/stage/cliff.mv1")[0]);

    //草
    MV1DrawModel(lpModelMng.GetID("./resource/model/stage/grass.mv1")[0]);

    //水
    origShader_->Draw(lpModelMng.GetID("./resource/model/stage/water100.mv1")[0], cubeTexture_);

    //UI
    resultUI_->Draw();

#ifdef _DEBUG
    //DrawFormatString(0, 0, 0xffffff, "x:%f\n,y:%f\n,z:%f\n", pos.x, pos.y, pos.z);
#endif	//_DEBUG
}

void ResultScene::Release(void)
{
    DeleteGraph(cubeTexture_);
}

void ResultScene::CreateCubeMap(void)
{
    //環境を描画する際に使用するカメラの注視点と上方向
    //注視点
    VECTOR targetPos[CUBE_TEXTURE_NUM] = {};
    targetPos[0] = VGet(1.0f, 0.0f, 0.0f);
    targetPos[1] = VGet(-1.0f, 0.0f, 0.0f);
    targetPos[2] = VGet(0.0f, 1.0f, 0.0f);
    targetPos[3] = VGet(0.0f, -1.0f, 0.0f);
    targetPos[4] = VGet(0.0f, 0.0f, 1.0f);
    targetPos[5] = VGet(0.0f, 0.0f, -1.0f);

    //上方向
    VECTOR upVec[CUBE_TEXTURE_NUM] = {};
    upVec[0] = VGet(0.0f, 1.0f, 0.0f);
    upVec[1] = VGet(0.0f, 1.0f, 0.0f);
    upVec[2] = VGet(0.0f, 0.0f, -1.0f);
    upVec[3] = VGet(0.0f, 0.0f, 1.0f);
    upVec[4] = VGet(0.0f, 1.0f, 0.0f);
    upVec[5] = VGet(0.0f, 1.0f, 0.0f);

    //環境を描画する面の数だけ繰り返す
    for (int i = 0; i < CUBE_TEXTURE_NUM; i++)
    {
        //SetDrawScreenと同じ
        SetRenderTargetToShader(0, cubeTexture_, i);
        ClearDrawScreen();

        //カメラの画角は90度
        SetupCamera_Perspective(Deg2RadF(90.0f));

        //範囲
        SetCameraNearFar(1.0f, 10000.0f);

        //カメラの設定
        SetCameraPositionAndTargetAndUpVec(VGet(0.0f, 0.0f, 0.0f), targetPos[i], upVec[i]);

        MV1DrawModel(lpModelMng.GetID("./resource/model/stage/dome/Skydome.mv1")[0]);
    }

    //60度に戻す
    SetupCamera_Perspective(Deg2RadF(60.0f));
}

UniqueScene ResultScene::UpdateScene(UniqueScene& scene)
{
    switch (resultUI_->GetHitUICol())
    {
    case ResultColUI::reTitle:
        return std::make_unique<TitleScene>();
        break;
    default:
        break;
    }
    //何もなければ元のシーンを返す
    return  std::move(scene);
}