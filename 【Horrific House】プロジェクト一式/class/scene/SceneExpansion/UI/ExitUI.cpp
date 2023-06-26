#include "ExitUI.h"
#include "../../../common/SceneID.h"
#include "../../SceneManager.h"
#include "../../../input/KeyInput.h"
#include "../../../input/PadInput.h"

ExitUI::ExitUI()
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
    //�V�[��ID�����Ȃ�������Max�������
    sceneID_ = SceneID::Max;
    //�A���œ��͂�����Ȃ��悤�ɍX�V
    controller_->Update();
    //������
    Init();
}

ExitUI::ExitUI(SceneID sceneID)
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
    //�O�̃V�[��ID
    sceneID_ = sceneID;
    //�A���œ��͂�����Ȃ��悤�ɍX�V
    controller_->Update();
    //������
    Init();
}

ExitUI::~ExitUI()
{
}

void ExitUI::Init()
{
    //tmx�ǂݍ���
    tmxObj_.LoadTmx("resource/tmx/exit.tmx", false);
    //Map������
    InitUIMap();
}

void ExitUI::Update()
{
    //�}�E�X�擾
    GetMousePoint(&mousePos_.x, &mousePos_.y);
    //�R���g���[���̍X�V
    controller_->Update();
    //�N���b�N���ē�������enum�i�[
    PutClickHit();
    //�}�E�X������������
    mouseHitCol_ = PutMouseHit(tmxObj_.GetExitCollList());
    //�t�F�[�h�C���A�E�g���X�V
    FadeInOutCnt_ += lpSceneMng.GetDeltaTime();
}

void ExitUI::Draw()
{
    for (auto UIObj : exitUIMap_)
    {
        if (mouseHitCol_ == UIObj.second.drawImage)
        {
            if (UIObj.second.CanHitUI != ExitColUI::Non)
            {
                //�I�������g�`��
                SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(std::sin(DX_PI * 2 / 60 *(80 * FadeInOutCnt_)) * 100 + 50));
                DrawExtendGraph(static_cast<int>(UIObj.second.coll.first.x) + 50,
                    static_cast<int>(UIObj.second.coll.first.y),
                    static_cast<int>(UIObj.second.coll.first.x) + static_cast<int>(UIObj.second.coll.second.x) - 50,
                    static_cast<int>(UIObj.second.coll.first.y) + static_cast<int>(UIObj.second.coll.second.y), lpImageMng.GetID("selection2")[0],
                    true);
                SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 100);
            }
        }
        //�����̕`��
        CollDrawCenterX(UIObj.second.coll, UIObj.second.drawImage);
    }
}

ExitColUI ExitUI::GetHitUICol()
{
    return decideHitCol_;
}

void ExitUI::InitUIMap()
{
    
    if (sceneID_ == SceneID::Option)
    {
        //�I�v�V�����������ꍇ
        exitUIMap_ =
        {
            //enum                               �摜,        �摜�̍��W ,tips�̉摜,         �����������ɕԂ�enum
            {ExitColUI::Title, UIObj<ExitColUI>("ExitToTitle",  {{0,0},{0,0}},"",         ExitColUI::Non)},
            {ExitColUI::YesOption, UIObj<ExitColUI>("Yes",  {{0,0},{0,0}},"",           ExitColUI::YesOption)},
            {ExitColUI::NoOption,  UIObj<ExitColUI>("No",   {{0,0},{0,0}},"",         ExitColUI::NoOption) }
        };
    }
    else
    {
        //����ȊO��������
        exitUIMap_ =
        {
            //enum                               �摜,        �摜�̍��W ,tips�̉摜,         �����������ɕԂ�enum
            {ExitColUI::Title, UIObj<ExitColUI>("ExitTitle",  {{0,0},{0,0}},"",         ExitColUI::Non)},
            {ExitColUI::Yes, UIObj<ExitColUI>("Yes",  {{0,0},{0,0}},"",           ExitColUI::Yes)},
            {ExitColUI::No,  UIObj<ExitColUI>("No",   {{0,0},{0,0}},"",         ExitColUI::No) }
        };
    }
    //tmx�������Ă����l��UIMap�ɑ������
    for (auto& coll : tmxObj_.GetExitCollList())
    {
        for (auto& UI : exitUIMap_)
        {
            if (coll.first == UI.second.drawImage)
            {
                //�摜�̍��W��tmx�������Ă������W������
                UI.second.coll = { coll.second.first,coll.second.second };
            }
        }
    }
}

void ExitUI::PutClickHit()
{
    decideHitCol_ = ExitColUI::Non;
    if (controller_->CheckInputKey(KeyID::Decide))
    {
        for (auto& coll : exitUIMap_)
        {
            if (CheckCollMouse()(mousePos_, coll.second.coll))
            {
                //�I������enum���i�[
                decideHitCol_ = coll.second.CanHitUI;
            }
        }
    }
}