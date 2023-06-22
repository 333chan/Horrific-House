#include "ConnectedUI.h"
#include "../../../input/KeyInput.h"
#include "../../../input/PadInput.h"
#include "../../../tmx/TmxObj.h"
#include "../../../common/CheckHit.h"
#include "../../SceneManager.h"
#include "../../../common/manager/ImageMng.h"
#include "../../../common/manager/SoundMng.h"
ConnectedUI::ConnectedUI()
{
    if (GetJoypadNum() > 0)
    {
        controller_ = std::make_unique<PadInput>();
    }
    else
    {
        controller_ = std::make_unique<KeyInput>();
    }
    Init();
}

ConnectedUI::~ConnectedUI()
{
}

void ConnectedUI::Init()
{
    InitUIMap();
    cntType_ = CntType::Key;
    font_ = CreateFontToHandle("papyrus", 50, 3, DX_FONTTYPE_NORMAL);

}

void ConnectedUI::Update()
{
    FadeInOutCnt_ += lpSceneMng.GetDeltaTime();
    //�}�E�X�擾
    GetMousePoint(&mousePos_.x, &mousePos_.y);
    //�R���g���[���̍X�V
    controller_->Update();
    //�N���b�N����enum�i�[
    PutClickHit();
    //����������������󂯎��
    mouseHitCol_ = PutMouseHit(tmxObj_.GetConnectedCollList());
}

void ConnectedUI::Draw()
{
    DrawExtendGraph(0, 0, lpSceneMng.GetScreenSize().x, lpSceneMng.GetScreenSize().y, lpImageMng.GetID("bg")[0], false);
    for (auto UIObj : connetctedUIMap_)
    {
        if (mouseHitCol_ == UIObj.second.drawImage)
        {
            if (UIObj.second.CanHitUI != ConnectedColUI::Non)
            {
                //�I�������g�`��
                SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(std::sin(DX_PI * 2 / 60 * (111 * FadeInOutCnt_)) * 100 + 50));
                DrawExtendGraph(static_cast<int>(UIObj.second.coll.first.x),
                    static_cast<int>(UIObj.second.coll.first.y),
                    static_cast<int>(UIObj.second.coll.first.x) + static_cast<int>(UIObj.second.coll.second.x),
                    static_cast<int>(UIObj.second.coll.first.y) + static_cast<int>(UIObj.second.coll.second.y), lpImageMng.GetID("selection1")[0],
                    true);
                SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 100);
            }
        }
        if (UIObj.first == ConnectedColUI::Text1)
        {
            //text1�̏ꏊ�ɒǉ���ip��\������
            IPDATA IPAddress[16];
            GetMyIPAddress(IPAddress);
            DrawFormatStringToHandle(static_cast<int>(UIObj.second.coll.first.x) + static_cast<int>(UIObj.second.coll.second.x/3), static_cast<int>(UIObj.second.coll.first.y), 0xffffff, font_, "%d.%d.%d.%d", IPAddress->d1, IPAddress->d2, IPAddress->d3, IPAddress->d4);
        }
        //�����̕`��
        CollDrawCenterX(UIObj.second.coll, UIObj.second.drawImage);
    }
    
}

ConnectedColUI ConnectedUI::GetHitUICol()
{
    return decideHitCol_;
}

CntType ConnectedUI::GetCntType()
{
    return cntType_;
}

void ConnectedUI::InitUIMap()
{
    //tmx
    tmxObj_.LoadTmx("resource/tmx/selectAR.tmx", false);
    connetctedUIMap_ =
    {
        //enum                               �摜,        �摜�̍��W ,tips�̉摜,         �����������ɕԂ�enum
        {ConnectedColUI::Title,   UIObj<ConnectedColUI>("ar_title", {{0,0},{0,0}},"",ConnectedColUI::Non)},
        {ConnectedColUI::Text0,   UIObj<ConnectedColUI>("ar_text0",   {{0,0},{0,0}},"",ConnectedColUI::Non)},
        {ConnectedColUI::Text1,   UIObj<ConnectedColUI>("ar_text1",   {{0,0},{0,0}},"",ConnectedColUI::Non)},
        {ConnectedColUI::Text2 ,  UIObj<ConnectedColUI>("ar_text2",    {{0,0},{0,0}},"",ConnectedColUI::Non)},
        {ConnectedColUI::Image0 , UIObj<ConnectedColUI>("ar_image0",{{0,0},{0,0}},"",ConnectedColUI::Non)},
        {ConnectedColUI::Image1 , UIObj<ConnectedColUI>("ar_image1",{{0,0},{0,0}},"",ConnectedColUI::Non)},
        {ConnectedColUI::Image2 , UIObj<ConnectedColUI>("ar_image2",{{0,0},{0,0}},"",ConnectedColUI::Non)},
        {ConnectedColUI::And ,     UIObj<ConnectedColUI>("ar_and",{{0,0},{0,0}},"",ConnectedColUI::Non)},
        {ConnectedColUI::Cancel , UIObj<ConnectedColUI>("ar_cancel",{{0,0},{0,0}},"",ConnectedColUI::Cancel)},
    };
    //tmx�������Ă����l��UIMap�ɑ������
    for (auto& coll : tmxObj_.GetConnectedCollList())
    {
        for (auto& UI : connetctedUIMap_)
        {
            if (coll.first == UI.second.drawImage)
            {
                //�摜�̍��W��tmx�������Ă������W������
                UI.second.coll = { coll.second.first,coll.second.second };
            }
        }
    }

}

void ConnectedUI::PutClickHit()
{
    decideHitCol_ = ConnectedColUI::Non;
    if (controller_->CheckInputKey(KeyID::Decide))
    {
        for (auto& coll : connetctedUIMap_)
        {
            if (CheckCollMouse()(mousePos_, coll.second.coll))
            {
                //�I������enum���i�[
                decideHitCol_ = coll.second.CanHitUI;
            }
        }
    }
}