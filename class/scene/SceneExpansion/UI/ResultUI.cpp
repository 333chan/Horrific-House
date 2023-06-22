#include "../../SceneManager.h"
#include "../../../input/KeyInput.h"
#include "../../../input/PadInput.h"

#include "ResultUI.h"

constexpr double LERP_TIME = 1.0;

constexpr int CREATOR_POS_X = 960;
constexpr int CREATOR_POS_Y = 300;

constexpr int ROLE_POS_X = 960;
constexpr int ROLE_POS_Y = 0;
constexpr int ROLE_OFFSET_X = 250;
constexpr int ROLE_OFFSET_Y = 50;

constexpr double CRETATOR_INIT_WAIT_TIME = 0.5;
constexpr double STOP_TIME = 2.0;

ResultUI::ResultUI()
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
    Init();
}

ResultUI::~ResultUI()
{
}

void ResultUI::Init()
{
	tmxObj_.LoadTmx("resource/tmx/result.tmx", false);
    InitUIMap();

    creator_.pos = { lpSceneMng.GetScreenSize().x + CREATOR_POS_X,CREATOR_POS_Y };
    creator_.stepLerpTime = LERP_TIME;
    stepTime_ = 0.0;

    role_.pos = { lpSceneMng.GetScreenSize().x + ROLE_POS_X,ROLE_POS_Y };
    role_.stepLerpTime = LERP_TIME;
    role_.roleFlag = true;

    lastFlag_ = false;

    roleType_ = RoleType::PLAN;
}

void ResultUI::Update()
{
    //�}�E�X�擾
    GetMousePoint(&mousePos_.x, &mousePos_.y);
    //�R���g���[���̍X�V
    controller_->Update();
    //�N���b�N���ē�������enum�i�[
    PutClickHit();
    //����������������󂯎��
    mouseHitCol_ = PutMouseHit(tmxObj_.GetResultCollList());
    FadeInOutCnt_++;

    if (stepTime_ > CRETATOR_INIT_WAIT_TIME)
    {
        CreditProcess(creator_, { CREATOR_POS_X,CREATOR_POS_Y });
    }
    else
    {
        stepTime_ += lpSceneMng.GetDeltaTime();
    }
    CreditProcess(role_, { ROLE_POS_X,ROLE_POS_Y });
}

void ResultUI::Draw()
{
    for (const auto& UIObj : resultUIMap_)
    {
        if (mouseHitCol_ == UIObj.second.drawImage)
        {
            //CollDrawCenter(UIObj.second.coll, "selection2");
            if (UIObj.second.CanHitUI != ResultColUI::Non)
            {
                SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(std::sin(DX_PI * 2 / 60 * FadeInOutCnt_) * 100 + 50));
                DrawExtendGraph(static_cast<int>(UIObj.second.coll.first.x) + 50,
                    static_cast<int>(UIObj.second.coll.first.y),
                    static_cast<int>(UIObj.second.coll.first.x) + static_cast<int>(UIObj.second.coll.second.x) - 50,
                    static_cast<int>(UIObj.second.coll.first.y) + static_cast<int>(UIObj.second.coll.second.y), lpImageMng.GetID("selection2")[0],
                    true);
                SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 100);
            }

            CollDrawCenterX(tipsColl_, UIObj.second.tipsImage);
        }
        CollDrawCenterX(UIObj.second.coll, UIObj.second.drawImage);
    }

    int gx, gy;
    if (lastFlag_)
    {
        GetGraphSize(lpImageMng.GetID("resource/image/Font/clear/clear.png")[0], &gx, &gy);
        DrawRotaGraph(role_.pos.x, role_.pos.y + gy / 2 + ROLE_OFFSET_Y, 1.0, 0, lpImageMng.GetID("resource/image/Font/clear/clear.png")[0], true);
        DrawRotaGraph(role_.pos.x, role_.pos.y + gy*3/2 +ROLE_OFFSET_Y, 1.0, 0, lpImageMng.GetID("resource/image/Font/clear/last.png")[0], true);
    }
    else
    {
        //����
        std::string rolePath = "resource/image/Font/clear/role";
        rolePath += std::to_string(static_cast<int>(roleType_)) + ".png";
        GetGraphSize(lpImageMng.GetID(rolePath)[0], &gx, &gy);
        DrawRotaGraph(role_.pos.x - ROLE_OFFSET_X, role_.pos.y + gy / 2 + ROLE_OFFSET_Y, 1.0, 0, lpImageMng.GetID(rolePath)[0], true);

        //���O
        GetGraphSize(lpImageMng.GetID("resource/image/Font/clear/name.png")[0], &gx, &gy);
        DrawRotaGraph(creator_.pos.x, creator_.pos.y + gy / 4, 1.0, 0, lpImageMng.GetID("resource/image/Font/clear/name.png")[0], true);
    }
}

ResultColUI ResultUI::GetHitUICol()
{
    return decideHitCol_;
}

ControllerType ResultUI::GetControllerType()
{
    return controller_->GetControllerType();
}

void ResultUI::InitUIMap()
{
    resultUIMap_ =
    {
        //enum                               �摜,        �摜�̍��W ,tips�̉摜,         �����������ɕԂ�enum
        {ResultColUI::reTitle,  UIObj<ResultColUI>("resultTitle",   {{0,0},{0,0}},"spTitleReturn",         ResultColUI::reTitle) }
    };
    //tmx�������Ă����l��UIMap�ɑ������
    for (const auto& coll : tmxObj_.GetResultCollList())
    {
        for (auto& UI : resultUIMap_)
        {
            if (coll.first == UI.second.drawImage)
            {
                //�摜�̍��W��tmx�������Ă������W������
                UI.second.coll = { coll.second.first,coll.second.second };
            }
        }
        if (coll.first == "resultOperation")
        {
            //�q���g�悤�ɍ��W������Ă���
            tipsColl_ = coll.second;
        }
    }
}

void ResultUI::PutClickHit()
{
    decideHitCol_ = ResultColUI::Non;
    if (controller_->CheckInputKey(KeyID::Decide))
    {
        for (const auto& coll : resultUIMap_)
        {
            if (CheckCollMouse()(mousePos_, coll.second.coll))
            {
                //��������enum
                decideHitCol_ = coll.second.CanHitUI;
            }
        }
    }
}

void ResultUI::CreditProcess(CreditData& creditData, Vector2Int pos)
{
    //��~���ԓ��Ȃ�Ԃ�
    if (creditData.stepStopTime < STOP_TIME)
    {
        creditData.stepStopTime += lpSceneMng.GetDeltaTime();
        return;
    }

    //��Ԏ��Ԍv�Z
    double lerpTime = 1.0 - (creditData.stepLerpTime / LERP_TIME);

    Vector2Int sc = lpSceneMng.GetScreenSize();
    sc.x /= 2;

    int dirInt = static_cast<int>(creditData.dir);

    creditData.pos = Lerp(creditData.pos, { (lpSceneMng.GetScreenSize().x + pos.x) - pos.x * (dirInt * 2),pos.y }, lerpTime);

    if (lastFlag_)
    {
        return;
    }

    //�ړ����I����Ă���ꍇ
    //enum�X�V����
    if (lerpTime >= 1.0f)
    {
        creditData.stepLerpTime = LERP_TIME;
        dirInt++;

        if (dirInt >= static_cast<int>(CreditTargetDir::MAX))
        {
            creditData.pos = { lpSceneMng.GetScreenSize().x + pos.x,pos.y };
            dirInt = static_cast<int>(CreditTargetDir::CENTER);

            //�����̏ꍇ�͕����ύX����������
            if (creditData.roleFlag)
            {
                int roleTypeInt = static_cast<int>(roleType_);
                roleTypeInt++;
                if (roleTypeInt >= static_cast<int>(RoleType::MAX))
                {
                    //roleTypeInt = static_cast<int>(RoleType::PLAN);
                    lastFlag_ = true;
                }
                roleType_ = static_cast<RoleType>(roleTypeInt);
            }
        }
        creditData.dir = static_cast<CreditTargetDir>(dirInt);
        if (creditData.dir == CreditTargetDir::LEFT)
        {
            creditData.stepStopTime = 0.0;
        }
    }
    creditData.stepLerpTime -= lpSceneMng.GetDeltaTime();
}
