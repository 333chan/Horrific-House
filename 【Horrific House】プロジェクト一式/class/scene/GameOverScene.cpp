#include "GameOverScene.h"
//#include "GameScene.h"
#include "TitleScene.h"
#include "LoadScene.h"
#include"../camera/Camera.h"
#include "../input/PadInput.h"
#include "../input/KeyInput.h"
#include "../tmx/TmxObj.h"
#include "../common/manager/ImageMng.h"
#include "../common/CheckHit.h"
#include "SceneManager.h"
#include "../common/OriginalShader.h"
#include "../common/manager/ModelMng.h"
#include"../object/enemy/Enemy.h"
#include "SceneExpansion/buffer/GameOverCBuffer.h"
#include "../common/manager/SoundMng.h"
#include "SceneExpansion//UI/GameOverUI.h"
#include "../common/AnimationController.h"
#include "../common/MouseIcon.h"

//constexpr int DRAW_OFFSET = 50;
constexpr int DIR_CAMERA_POS_X = -100000;//演出用カメラのX座標
constexpr int DIR_CAMERA_POS_Y = -10000;//演出用カメラのY座標
constexpr int DIR_CAMERA_POS_Z = -11500;//演出用カメラのZ座標
constexpr int DIR_CAMERA_OFFSET_Y = -10000 + 300;//演出用カメラのY座標オフセット
constexpr int GHOST_POS_X = -100000;//ghostのX座標
constexpr int GHOST_POS_YZ = -10000;//ghostのYZ座標
constexpr float GHOST_SCALE = 3.7f;//ghostの大きさ
constexpr float SOUND_SCALE = 10000.0f;//サウンドの聞こえる範囲
constexpr int DIRCAM_MOVESPEED_Z = 50;//演出用カメラの移動スピード
constexpr int DIRCAM_MOVESPEED_Y = 20;//演出用カメラの移動スピード


GameOverScene::GameOverScene( ControllerType type,  Vector3 playerPos, Vector3 enemyPos,Vector3 cameraAngle,CntType cntType)
{
    //beforeScene_ = std::move(ownScene);
    //beforeSceneID_ = beforeID;
    cntType_ = cntType;
    if (type == ControllerType::Pad)
    {
        controller_ = std::make_unique<PadInput>();
    }
    else
    {
        controller_ = std::make_unique<KeyInput>();
    }
    //カメラ
    camera_ = std::make_unique<Camera>(cameraAngle);
    
    enemy_ = std::make_unique<Enemy>(enemyPos);	//敵

    gameOverUI_ = std::make_unique<GameOverUI>();

    playerPos_ = playerPos;//プレイヤーの死んだ位置
    enemyPos_ = enemyPos;//敵の位置
   
    Init();
    
    DrawScreen();
}

GameOverScene::~GameOverScene()
{
	Release();
}

void GameOverScene::Init(void)
{
    //ロード
    tmxObj_.LoadTmx("resource/tmx/gameover.tmx", false);
    
    //srand(static_cast<unsigned int>(time(nullptr)));
    brightness_ = lpSceneMng.GetBrightnessScrren();//輝度
    Cnt_ = 0.0;//死亡演出の表示カウント
    directionEndflg_ = false;
    //シェーダ関連
    cBuffer_ = std::make_unique<GameOverCBuffer>();	//ディゾルブ
    int VertexShader = LoadVertexShader("resource/shader/vs/NormalModel_vs.vso");
    int PixelShader = LoadPixelShader("resource/shader/ps/GameModel_ps.pso");
    //int PixelShader = LoadPixelShader("resource/shader/ps/renderGBuffer.pso");

    int PixelShader2 = LoadPixelShader("resource/Shader/ps/Dissolve_ps.pso");
    origShader_ = std::make_unique<OriginalShader>(VertexShader, PixelShader);
    origShader2_ = std::make_unique<OriginalShader>(PixelShader2);
    renderTarget_ = MakeScreen(lpSceneMng.GetScreenSize().x, lpSceneMng.GetScreenSize().y);
    renderTarget2_ = MakeScreen(lpSceneMng.GetScreenSize().x, lpSceneMng.GetScreenSize().y);

    //-演出用ghost関連の初期化-
    ghostPos_ = Vector3{ GHOST_POS_X, GHOST_POS_YZ, GHOST_POS_YZ };
    animController_ = std::make_unique<AnimationController>(lpModelMng.GetID("./resource/model/enemy/ghost3.mv1")[1]);
    animController_->Add((int)EnemyAnimation::Idle, "./resource/model/enemy/anim/Idle.mv1", 40.0f);
    animController_->Add((int)EnemyAnimation::Scream, "./resource/model/enemy/anim/sakebu.mv1", 80.0f);
    animController_->Play((int)EnemyAnimation::Scream, false);

    MV1SetPosition(lpModelMng.GetID("./resource/model/enemy/ghost3.mv1")[1], VGet(ghostPos_.x, ghostPos_.y, ghostPos_.z));
    MV1SetScale(lpModelMng.GetID("./resource/model/enemy/ghost3.mv1")[1], { GHOST_SCALE,GHOST_SCALE,GHOST_SCALE });
    //--------------------------
    //--演出用カメラ関連の初期化--
    dirCameraPos_ = Vector3{ DIR_CAMERA_POS_X,DIR_CAMERA_POS_Y, DIR_CAMERA_POS_Z };
    dirCameraPosOffset_.y = DIR_CAMERA_OFFSET_Y;
    dirCameraTargetPos_ = ghostPos_;
    flag_ = false;

    //ポストエフェクト
    psPostEffect_ = LoadPixelShader("./resource/shader/ps/horrorEffect_ps.pso");
    postEffect_ = std::make_unique<OriginalShader>(psPostEffect_);

    //音
    lpSoundMng.AllStopSound();
    PlaySoundMem(lpSoundMng.GetID("./resource/sound/horrornoise.mp3")[0], DX_PLAYTYPE_LOOP);
   
}

UniqueScene GameOverScene::Update(UniqueScene scene)
{
    UpdateDirection();//演出の更新
   
    DrawScreen();
	return UpdateScene(scene);
}

void GameOverScene::DrawScreen(void)
{
    SetDrawScreen(renderTarget_);
    ClsDrawScreen();

    DrawDirection();

}

void GameOverScene::Release(void)
{
}

void GameOverScene::SetRenderTarget(int screenID)
{
    SetDrawScreen(renderTarget_);
    ClsDrawScreen();
    MV1DrawModel(lpModelMng.GetID("resource/model/stage/Stage1.mv1")[0]);
    //origShader_->Draw(lpModelMng.GetID("resource/model/stage/Stage1.mv1")[0]);        
    enemy_->Draw();
    origShader2_->DrawOnlyPixel(0.0f, 0.0f, screenID, lpImageMng.GetID("./resource/shader/ps/texture/noise7.png")[0], -1, -1);
}

UniqueScene GameOverScene::UpdateScene(UniqueScene& scene)
{
    switch (gameOverUI_->GetHitUICol())
    {
    case GameOverColUI::ToTitle:
        return std::make_unique<TitleScene>();
        break;
    case GameOverColUI::Restart:
        return std::make_unique<LoadScene>(cntType_);
        break; 
    default:
        break;
    }
    return  std::move(scene);
}

void GameOverScene::UpdateDirection()
{
    animController_->Update(lpSceneMng.GetDeltaTime());

    if (directionEndflg_)//演出が終わっていたら
    {
        if (WaitCnt(8.0))
        {
            controller_->Update();//入力情報の更新
            gameOverUI_->Update();
            //TRACE("volume %d\n", Volume_/2);
            lpSceneMng.SetBrightnessScrren(brightness_);
        }
    }
    //敵の更新
    enemy_->Update();
    enemy_->PKMotion(dirCameraPos_);
   
    if (!WaitCnt(2.0))//一定時間経つまで
    {
        if (dirCameraPos_.y < dirCameraPosOffset_.y)//一定の距離まで近づく
        {
            if (!CheckSoundMem(lpSoundMng.GetID("./resource/sound/RunVoice.mp3")[0]))
            {
                lpSoundMng.Play3DSound("./resource/sound/RunVoice.mp3", ghostPos_, SOUND_SCALE);
            }
            dirCameraPos_.z += DIRCAM_MOVESPEED_Z;
            dirCameraPos_.y += DIRCAM_MOVESPEED_Y;
            dirCameraTargetPos_.y += DIRCAM_MOVESPEED_Y;
        }
        else//その場にとどまり、敵のモーションをIDLEに変える
        {
            if (WaitCnt(1.3))//一定時間経っていたら
            {
                animController_->Play((int)EnemyAnimation::Idle, true);
            }
        }
       
    }
    else//一定時間たったら
    {
        lpSoundMng.StopSound("./resource/sound/RunVoice.mp3");
        lpSoundMng.StopSound("./resource/sound/HeatBeat.mp3");
        lpSoundMng.StopSound("./resource/sound/horrornoise.mp3");
        
        //死亡演出が終わっていたら
        dirCameraPos_ = playerPos_;//カメラを死亡した位置へ
        camera_->Update(enemyPos_);
        
    }
    if (WaitCnt(4.0))//一定時間たったら
    {
        cBuffer_->Update();//ディゾルブの更新
        if (!directionEndflg_)
        {
            lpSoundMng.PlayingSound("./resource/sound/gameoverBGM.mp3");
            directionEndflg_ = true;//演出終了フラグ
        }
    }
    cBuffer_->Update(Cnt_);//ノイズの更新
    Cnt_ += lpSceneMng.GetDeltaTime();//経過時間を変数にセット
    lpMouseIcon.SetFlg(directionEndflg_);
}

void GameOverScene::DrawDirection()
{
    if (!WaitCnt(2.0))
    {
        camera_->SetBeforeDraw_dead(dirCameraPos_, dirCameraTargetPos_);//プレイヤーの死亡した位置にカメラを描画
        MV1DrawModel(lpModelMng.GetID("./resource/model/enemy/ghost3.mv1")[1]);
    }
    else
    {
        camera_->SetBeforeDraw_dead(dirCameraPos_, enemyPos_);//プレイヤーの死亡した位置にカメラを描画
    }

    MV1DrawModel(lpModelMng.GetID("resource/model/stage/Stage1.mv1")[0]);
    origShader_->Draw(lpModelMng.GetID("resource/model/stage/Stage1.mv1")[0]);
    enemy_->Draw();

    SetDrawScreen(renderTarget2_);
    ClsDrawScreen();

    DrawExtendGraph(0,
        0,
        lpSceneMng.GetScreenSize().x,
        lpSceneMng.GetScreenSize().y,
        lpImageMng.GetID("resource/image/gameOver/horrorBg.jpg")[0],
        true);
    gameOverUI_->Draw();

    SetDrawScreen(screenID_);
    ClsDrawScreen();

    origShader2_->DrawOnlyPixel(0.0f, 0.0f, renderTarget_, lpImageMng.GetID("./resource/shader/ps/texture/disNoise.png")[0], renderTarget2_, -1);
    postEffect_->DrawOnlyPixel(0.0f, 0.0f, renderTarget_, lpImageMng.GetID("./resource/shader/ps/texture/disNoise.png")[0], -1, -1);

}

bool GameOverScene::WaitCnt(double waitCnt)
{
    return Cnt_ > waitCnt;//指定した待ち時間が経過していたらtrueを返す。
}