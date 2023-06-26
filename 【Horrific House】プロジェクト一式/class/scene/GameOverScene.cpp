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
constexpr int DIR_CAMERA_POS_X = -100000;//���o�p�J������X���W
constexpr int DIR_CAMERA_POS_Y = -10000;//���o�p�J������Y���W
constexpr int DIR_CAMERA_POS_Z = -11500;//���o�p�J������Z���W
constexpr int DIR_CAMERA_OFFSET_Y = -10000 + 300;//���o�p�J������Y���W�I�t�Z�b�g
constexpr int GHOST_POS_X = -100000;//ghost��X���W
constexpr int GHOST_POS_YZ = -10000;//ghost��YZ���W
constexpr float GHOST_SCALE = 3.7f;//ghost�̑傫��
constexpr float SOUND_SCALE = 10000.0f;//�T�E���h�̕�������͈�
constexpr int DIRCAM_MOVESPEED_Z = 50;//���o�p�J�����̈ړ��X�s�[�h
constexpr int DIRCAM_MOVESPEED_Y = 20;//���o�p�J�����̈ړ��X�s�[�h


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
    //�J����
    camera_ = std::make_unique<Camera>(cameraAngle);
    
    enemy_ = std::make_unique<Enemy>(enemyPos);	//�G

    gameOverUI_ = std::make_unique<GameOverUI>();

    playerPos_ = playerPos;//�v���C���[�̎��񂾈ʒu
    enemyPos_ = enemyPos;//�G�̈ʒu
   
    Init();
    
    DrawScreen();
}

GameOverScene::~GameOverScene()
{
	Release();
}

void GameOverScene::Init(void)
{
    //���[�h
    tmxObj_.LoadTmx("resource/tmx/gameover.tmx", false);
    
    //srand(static_cast<unsigned int>(time(nullptr)));
    brightness_ = lpSceneMng.GetBrightnessScrren();//�P�x
    Cnt_ = 0.0;//���S���o�̕\���J�E���g
    directionEndflg_ = false;
    //�V�F�[�_�֘A
    cBuffer_ = std::make_unique<GameOverCBuffer>();	//�f�B�]���u
    int VertexShader = LoadVertexShader("resource/shader/vs/NormalModel_vs.vso");
    int PixelShader = LoadPixelShader("resource/shader/ps/GameModel_ps.pso");
    //int PixelShader = LoadPixelShader("resource/shader/ps/renderGBuffer.pso");

    int PixelShader2 = LoadPixelShader("resource/Shader/ps/Dissolve_ps.pso");
    origShader_ = std::make_unique<OriginalShader>(VertexShader, PixelShader);
    origShader2_ = std::make_unique<OriginalShader>(PixelShader2);
    renderTarget_ = MakeScreen(lpSceneMng.GetScreenSize().x, lpSceneMng.GetScreenSize().y);
    renderTarget2_ = MakeScreen(lpSceneMng.GetScreenSize().x, lpSceneMng.GetScreenSize().y);

    //-���o�pghost�֘A�̏�����-
    ghostPos_ = Vector3{ GHOST_POS_X, GHOST_POS_YZ, GHOST_POS_YZ };
    animController_ = std::make_unique<AnimationController>(lpModelMng.GetID("./resource/model/enemy/ghost3.mv1")[1]);
    animController_->Add((int)EnemyAnimation::Idle, "./resource/model/enemy/anim/Idle.mv1", 40.0f);
    animController_->Add((int)EnemyAnimation::Scream, "./resource/model/enemy/anim/sakebu.mv1", 80.0f);
    animController_->Play((int)EnemyAnimation::Scream, false);

    MV1SetPosition(lpModelMng.GetID("./resource/model/enemy/ghost3.mv1")[1], VGet(ghostPos_.x, ghostPos_.y, ghostPos_.z));
    MV1SetScale(lpModelMng.GetID("./resource/model/enemy/ghost3.mv1")[1], { GHOST_SCALE,GHOST_SCALE,GHOST_SCALE });
    //--------------------------
    //--���o�p�J�����֘A�̏�����--
    dirCameraPos_ = Vector3{ DIR_CAMERA_POS_X,DIR_CAMERA_POS_Y, DIR_CAMERA_POS_Z };
    dirCameraPosOffset_.y = DIR_CAMERA_OFFSET_Y;
    dirCameraTargetPos_ = ghostPos_;
    flag_ = false;

    //�|�X�g�G�t�F�N�g
    psPostEffect_ = LoadPixelShader("./resource/shader/ps/horrorEffect_ps.pso");
    postEffect_ = std::make_unique<OriginalShader>(psPostEffect_);

    //��
    lpSoundMng.AllStopSound();
    PlaySoundMem(lpSoundMng.GetID("./resource/sound/horrornoise.mp3")[0], DX_PLAYTYPE_LOOP);
   
}

UniqueScene GameOverScene::Update(UniqueScene scene)
{
    UpdateDirection();//���o�̍X�V
   
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

    if (directionEndflg_)//���o���I����Ă�����
    {
        if (WaitCnt(8.0))
        {
            controller_->Update();//���͏��̍X�V
            gameOverUI_->Update();
            //TRACE("volume %d\n", Volume_/2);
            lpSceneMng.SetBrightnessScrren(brightness_);
        }
    }
    //�G�̍X�V
    enemy_->Update();
    enemy_->PKMotion(dirCameraPos_);
   
    if (!WaitCnt(2.0))//��莞�Ԍo�܂�
    {
        if (dirCameraPos_.y < dirCameraPosOffset_.y)//���̋����܂ŋ߂Â�
        {
            if (!CheckSoundMem(lpSoundMng.GetID("./resource/sound/RunVoice.mp3")[0]))
            {
                lpSoundMng.Play3DSound("./resource/sound/RunVoice.mp3", ghostPos_, SOUND_SCALE);
            }
            dirCameraPos_.z += DIRCAM_MOVESPEED_Z;
            dirCameraPos_.y += DIRCAM_MOVESPEED_Y;
            dirCameraTargetPos_.y += DIRCAM_MOVESPEED_Y;
        }
        else//���̏�ɂƂǂ܂�A�G�̃��[�V������IDLE�ɕς���
        {
            if (WaitCnt(1.3))//��莞�Ԍo���Ă�����
            {
                animController_->Play((int)EnemyAnimation::Idle, true);
            }
        }
       
    }
    else//��莞�Ԃ�������
    {
        lpSoundMng.StopSound("./resource/sound/RunVoice.mp3");
        lpSoundMng.StopSound("./resource/sound/HeatBeat.mp3");
        lpSoundMng.StopSound("./resource/sound/horrornoise.mp3");
        
        //���S���o���I����Ă�����
        dirCameraPos_ = playerPos_;//�J���������S�����ʒu��
        camera_->Update(enemyPos_);
        
    }
    if (WaitCnt(4.0))//��莞�Ԃ�������
    {
        cBuffer_->Update();//�f�B�]���u�̍X�V
        if (!directionEndflg_)
        {
            lpSoundMng.PlayingSound("./resource/sound/gameoverBGM.mp3");
            directionEndflg_ = true;//���o�I���t���O
        }
    }
    cBuffer_->Update(Cnt_);//�m�C�Y�̍X�V
    Cnt_ += lpSceneMng.GetDeltaTime();//�o�ߎ��Ԃ�ϐ��ɃZ�b�g
    lpMouseIcon.SetFlg(directionEndflg_);
}

void GameOverScene::DrawDirection()
{
    if (!WaitCnt(2.0))
    {
        camera_->SetBeforeDraw_dead(dirCameraPos_, dirCameraTargetPos_);//�v���C���[�̎��S�����ʒu�ɃJ������`��
        MV1DrawModel(lpModelMng.GetID("./resource/model/enemy/ghost3.mv1")[1]);
    }
    else
    {
        camera_->SetBeforeDraw_dead(dirCameraPos_, enemyPos_);//�v���C���[�̎��S�����ʒu�ɃJ������`��
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
    return Cnt_ > waitCnt;//�w�肵���҂����Ԃ��o�߂��Ă�����true��Ԃ��B
}