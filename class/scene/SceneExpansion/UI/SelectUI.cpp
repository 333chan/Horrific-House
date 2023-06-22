#include "SelectUI.h"

#include "../../../input/KeyInput.h"
#include "../../../input/PadInput.h"
#include "../../../tmx/TmxObj.h"
#include "../../../common/CheckHit.h"
#include "../../SceneManager.h"
#include "../../../common/manager/ImageMng.h"

SelectUI::SelectUI()
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
    //�R���g���[���̍X�V
    controller_->Update();
    Init();
}

SelectUI::~SelectUI()
{
}

void SelectUI::Init()
{
    InitUIMap();
    cntType_ = CntType::Key;
}

void SelectUI::Update()
{    
    FadeInOutCnt_ += lpSceneMng.GetDeltaTime();
    //�}�E�X�擾
    GetMousePoint(&mousePos_.x, &mousePos_.y);
    //�R���g���[���̍X�V
    controller_->Update();
    //�N���b�N���ē�������enum�i�[
    PutClickHit();
    //����������������󂯎��
    mouseHitCol_ = PutMouseHit(tmxObj_.GetSelectCollList());
}

void SelectUI::Draw()
{
    DrawExtendGraph(0, 0, lpSceneMng.GetScreenSize().x, lpSceneMng.GetScreenSize().y, lpImageMng.GetID("bg")[0], false);
    for (auto UIObj : selectUIMap_)
    {
        if (mouseHitCol_ == UIObj.second.drawImage)
        {
            if (UIObj.second.CanHitUI != SelectColUI::Non)
            {
                //�}�E�X�ɓ������Ă��鏊�ɘg�`��
                SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(std::sin(DX_PI * 2 / 60 * (111 * FadeInOutCnt_)) * 100 + 50));                
                DrawExtendGraph(static_cast<int>(UIObj.second.coll.first.x),
                    static_cast<int>(UIObj.second.coll.first.y),
                    static_cast<int>(UIObj.second.coll.first.x) + static_cast<int>(UIObj.second.coll.second.x),
                    static_cast<int>(UIObj.second.coll.first.y) + static_cast<int>(UIObj.second.coll.second.y), lpImageMng.GetID("selection1")[0],
                    true);
                SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 100);
            }
            //�q���g�̕`��
            CollDrawCenterX(tipsColl_, UIObj.second.tipsImage);
        }
        if (cntType_ == UIObj.second.type && cntType_ != CntType::Non)
        {
            //�f�o�C�X��I�������g�`��
            SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(std::sin(DX_PI * 2 / 60 * (30 * FadeInOutCnt_)) * 100 +100));
            DrawExtendGraph(static_cast<int>(UIObj.second.coll.first.x),
            static_cast<int>(UIObj.second.coll.first.y - 20),
                static_cast<int>(UIObj.second.coll.first.x) + static_cast<int>(UIObj.second.coll.second.x),
                static_cast<int>(UIObj.second.coll.first.y) + static_cast<int>(UIObj.second.coll.second.y), lpImageMng.GetID("selection")[0],
                true);
            SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 100);
        }
        //�f�o�C�X���摜�`��
        DrawExtendGraph(static_cast<int>(UIObj.second.coll.first.x + 50),
            static_cast<int>(UIObj.second.coll.first.y+70),
            static_cast<int>(UIObj.second.coll.first.x) + static_cast<int>(UIObj.second.coll.second.x) - 50,
            static_cast<int>(UIObj.second.coll.first.y) + static_cast<int>(UIObj.second.coll.second.y)-20, lpImageMng.GetID(UIObj.second.deviceImage)[0],
            true);
        //�����̕`��
        CollDrawCenterX(UIObj.second.coll, UIObj.second.drawImage);
    }
}

SelectColUI SelectUI::GetHitUICol()
{
	return decideHitCol_;
}

CntType SelectUI::GetCntType()
{
    return cntType_;
}

void SelectUI::InitUIMap()
{
    //tmx
    tmxObj_.LoadTmx("resource/tmx/select.tmx", false);
    selectUIMap_ =
    {
        //enum                               �摜,        �摜�̍��W ,tips�̉摜,         �����������ɕԂ�enum
        {SelectColUI::Title, UISelectObj<SelectColUI>(  "select_title", {{0,0},{0,0}},"select_title_tip", "",               CntType::Non,                 SelectColUI::Non)},
        {SelectColUI::Key, UISelectObj<SelectColUI>(    "select_key",   {{0,0},{0,0}},"select_key_tip",   "desktopPc_Image",CntType::Key,                 SelectColUI::Non)},
        {SelectColUI::Pad, UISelectObj<SelectColUI>(    "select_pad",   {{0,0},{0,0}},"select_pad_tip",   "gamePad_Image",  CntType::Pad,                 SelectColUI::Non)},
        {SelectColUI::Ar , UISelectObj<SelectColUI>(    "select_ar",    {{0,0},{0,0}},"select_ar_tip",    "smh_Image",      CntType::Smh,                 SelectColUI::Non)},
        {SelectColUI::Decide , UISelectObj<SelectColUI>("select_decide",{{0,0},{0,0}},"select_decide_tip","",               CntType::Non,                 SelectColUI::Decide)},
        {SelectColUI::Return , UISelectObj<SelectColUI>("select_return",{{0,0},{0,0}},"select_return_tip","",               CntType::Non,                 SelectColUI::Return)},
    };
    //tmx�������Ă����l��UIMap�ɑ������
    for (auto& coll : tmxObj_.GetSelectCollList())
    {
        for (auto& UI : selectUIMap_)
        {
            if (coll.first == UI.second.drawImage)
            {
                //�摜�̍��W��tmx�������Ă������W������
                UI.second.coll = { coll.second.first,coll.second.second };
            }
        }
        if (coll.first == "operation")
        {
            //�q���g�悤�ɍ��W������Ă���
            tipsColl_ = coll.second;
        }
    }
    
}

void SelectUI::PutClickHit()
{
    decideHitCol_ = SelectColUI::Non;
    if (controller_->CheckInputKey(KeyID::Decide))
    {
        for (auto& coll : selectUIMap_)
        {
            if (CheckCollMouse()(mousePos_, coll.second.coll))
            {
                //�I������enum��Ԃ�
                decideHitCol_ = coll.second.CanHitUI;
                if (coll.second.type != CntType::Non)
                {
                    //�t�F�[�h�C����������
                    FadeInOutCnt_ = 0;
                    //�N���b�N�f�o�C�X�̃^�C�v���i�[
                    cntType_ = coll.second.type;
                }
            }
        }
    }
}
