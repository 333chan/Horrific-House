#include "../../SceneManager.h"
#include "../../../common/manager/ImageMng.h"
#include "../../../input/KeyInput.h"
#include "../../../input/PadInput.h"
#include "../../../input/Controller.h"
#include "../../../tmx/TmxObj.h"
#include "../../../common/CheckHit.h"

#include "GameOverUI.h"

constexpr double RAD_ALPHA = (DX_PI * 2 / 60);
constexpr double MAGNIFICATION = 150;
constexpr int UI_POS_OFFSET = 50;

GameOverUI::GameOverUI()
{
    Init();
}

GameOverUI::~GameOverUI()
{
}

void GameOverUI::Init()
{
    //tmx
    tmxObj_.LoadTmx("resource/tmx/gameover.tmx", false);
    //�p�b�h�̐����P�ȏ�̏ꍇ�̓p�b�h����ɂ���
    if (GetJoypadNum() > 0)
    {
        controller_ = std::make_unique<PadInput>();
    }
    else
    {
        controller_ = std::make_unique<KeyInput>();
    }
   //map������
    InitUIMap();
}

void GameOverUI::Update()
{
    //�}�E�X�擾
    GetMousePoint(&mousePos_.x, &mousePos_.y);
    //�R���g���[���̍X�V
    controller_->Update();
    //�N���b�N���ē�������enum�i�[
    PutClickHit();
    //����������������󂯎��
    mouseHitCol_ = PutMouseHit(tmxObj_.GetGameoverCollList());
    FadeInOutCnt_++;
}

void GameOverUI::Draw()
{
    for (auto UIObj : gameoverUIMap_)
    {
        if (mouseHitCol_ == UIObj.second.drawImage)
        {
            if (UIObj.second.CanHitUI != GameOverColUI::Non)
            {
                //�I�������g�`��
                SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(std::sin(RAD_ALPHA * FadeInOutCnt_) * MAGNIFICATION));
                DrawExtendGraph(static_cast<int>(UIObj.second.coll.first.x + UI_POS_OFFSET),
                    static_cast<int>(UIObj.second.coll.first.y),
                    static_cast<int>(UIObj.second.coll.first.x) + static_cast<int>(UIObj.second.coll.second.x) - UI_POS_OFFSET,
                    static_cast<int>(UIObj.second.coll.first.y) + static_cast<int>(UIObj.second.coll.second.y), lpImageMng.GetID("selectB")[0],
                    true);
                SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 100);
            }
            //�q���g�̕`��
            CollDrawCenterX(tipsColl_, UIObj.second.tipsImage);
        }
        //�����̕`��
        CollDrawCenterX(UIObj.second.coll, UIObj.second.drawImage);
    }

}

void GameOverUI::InitUIMap()
{
    gameoverUIMap_ =
    {
        {GameOverColUI::GameOver, UIObj<GameOverColUI>("gameover",{{0,0},{0,0}},"",GameOverColUI::Non)},
        {GameOverColUI::ToTitle,  UIObj<GameOverColUI>("toTitleTag", {{0,0},{0,0}},"spTitleReturn",GameOverColUI::ToTitle) },
        {GameOverColUI::Restart,  UIObj<GameOverColUI>("reStartTag",{{0,0},{0,0}},"spReturn",GameOverColUI::Restart) },
    };
    //tmx�������Ă����l��UIMap�ɑ������
    for (auto& coll : tmxObj_.GetGameoverCollList())
    {
        for (auto& UI : gameoverUIMap_)
        {
            if (coll.first == UI.second.drawImage)
            {
                UI.second.coll = { coll.second.first,coll.second.second };
            }
        }
        //�I�v�V�����p�̃R���W�������Ƃ��Ēu��
        if (coll.first == "operation")
        {
            tipsColl_ = coll.second;
        }
    }
}

void GameOverUI::PutClickHit()
{
    decideHitCol_ = GameOverColUI::Non;
    if (controller_->CheckInputKey(KeyID::Decide))
    {
        for (auto& coll : gameoverUIMap_)
        {
            if (CheckCollMouse()(mousePos_, coll.second.coll))
            {
                //�N���b�N����������������
                decideHitCol_ = coll.second.CanHitUI;
            }
        }
    }
}

GameOverColUI GameOverUI::GetHitUICol()
{
    return decideHitCol_;
}