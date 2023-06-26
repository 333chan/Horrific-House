#include "PreTitleUI.h"
#include "../../SceneManager.h"
#include "../../../input/Controller.h"
#include "../../../input/KeyInput.h"
#include "../../../input/PadInput.h"
PreTitleUI::PreTitleUI()
{
    Init();
}

PreTitleUI::~PreTitleUI()
{
}

void PreTitleUI::Init()
{
    //�p�b�h�̐����P�ȏ�̏ꍇ�̓p�b�h����ɂ���
    if (GetJoypadNum() > 0)
    {
        controller_ = std::make_unique<PadInput>();
    }
    else
    {
        controller_ = std::make_unique<KeyInput>();
    }
    InitUIMap();
}

void PreTitleUI::Update()
{
    //�}�E�X�擾
    GetMousePoint(&mousePos_.x, &mousePos_.y);
    //�R���g���[���̍X�V
    controller_->Update();
    //�N���b�N���ē�������enum�i�[
    PutClickHit();
    FadeInOutCnt_++;
}

void PreTitleUI::Draw()
{
    for (auto UIObj : titleUIMap_)
    {
        if (UIObj.first == TitleColUI::Title)
        {
            //�^�C�g�������`��
            CollDrawCenterX(UIObj.second.coll, UIObj.second.drawImage);
        }
    }
    //���S�ɕ`��
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>((std::sin(DX_PI * 2 / 100 * FadeInOutCnt_) * 300 + 100) + 300));
    Vector2Int GraphSize;
    GetGraphSize(lpImageMng.GetID("anyButton")[0], &GraphSize.x, &GraphSize.y);
    GraphSize.x /= 2;
    GraphSize.y /= 2;
    DrawGraph(lpSceneMng.GetScreenSize().x / 2 - GraphSize.x, static_cast<int>(lpSceneMng.GetScreenSize().y / 1.5) - GraphSize.y, lpImageMng.GetID("anyButton")[0], true);
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

}

void PreTitleUI::InitUIMap()
{
    //tmx
    tmxObj_.LoadTmx("resource/tmx/title.tmx", false);
    titleUIMap_ =
    {
        //enum                               �摜,        �摜�̍��W ,tips�̉摜,         �����������ɕԂ�enum
        {TitleColUI::Title, UIObj<TitleColUI>("title",  {{0,0},{0,0}},"",                 TitleColUI::Non)},
    };
    //tmx�������Ă����l��UIMap�ɑ������
    for (auto& coll : tmxObj_.GetTitleCollList())
    {
        for (auto& UI : titleUIMap_)
        {
            if (coll.first == UI.second.drawImage)
            {
                //�摜�̍��W��tmx�������Ă������W������
                UI.second.coll = { coll.second.first,coll.second.second };
            }
        }
    }
}

void PreTitleUI::PutClickHit()
{
    if (controller_->CheckInputKey(KeyID::Decide))
    {
        PreEndFlag_ = true;
    }
}
