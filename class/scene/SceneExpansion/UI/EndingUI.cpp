#include "EndingUI.h"
#include "../../../common/manager/ImageMng.h"
#include "../../SceneManager.h"
#include "../../../input/KeyInput.h"
#include "../../../input/PadInput.h"
#include "../../../tmx/TmxObj.h"

constexpr double WAIT_TIME = 5.0;

EndingUI::EndingUI()
{
    Init();
}

EndingUI::EndingUI(CntType cntType)
{
    cntType_ = cntType;

    switch (cntType_)
    {
    case CntType::Key:
        controller_ = std::make_unique<KeyInput>();
        break;
    case CntType::Pad:
        controller_ = std::make_unique<PadInput>();
        break;
    case CntType::Smh:
        if (GetJoypadNum() > 0)
        {
            controller_ = std::make_unique<PadInput>();
        }
        else
        {
            controller_ = std::make_unique<KeyInput>();
        }
        break;
    default:
        break;
    }
    Init();
}

EndingUI::~EndingUI()
{
}

void EndingUI::Init()
{
    InitUIMap();
    time_ = 0.0;
    progress_ = (int)EndingColUI::Non;
    isEndFlg = false;
}

void EndingUI::Update()
{

    controller_->Update();
    if (controller_->CheckInputKey(KeyID::Decide) || time_ > WAIT_TIME)
    {
        if (progress_ < (int)EndingColUI::Max - 1)
        {
            progress_++;
            time_ = 0;

        }
        else
        {
            isEndFlg = true;
        }
    }
    time_ += lpSceneMng.GetDeltaTime();
}

void EndingUI::Draw()
{

    for (auto UIObj : endingUIMap_)
    {
        if ((EndingColUI)progress_ == UIObj.first)
        {
            SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(std::sin(DX_PI * 2 / 10 * time_) * 255));
            CollDrawCenter(UIObj.second.coll, UIObj.second.drawImage);
            SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 100);
        }
    }
    // DrawFormatString(0, 0, 0xffffff, "progress:%d", progress_);
}

bool EndingUI::GetIsEndFlg()
{
    return isEndFlg;
}

int EndingUI::Getprogress()
{
    return progress_;
}

void EndingUI::InitUIMap()
{
    //tmx
    tmxObj_.LoadTmx("resource/tmx/ending.tmx", false);
    endingUIMap_ =
    {
        //enum                               画像,        画像の座標 ,tipsの画像,         当たった時に返すenum
        {EndingColUI::Non, UIObj<EndingColUI>("ending_non", {{0,0},{0,0}},"", EndingColUI::Non)},
        {EndingColUI::First , UIObj<EndingColUI>("ending_first",{{0,0},{0,0}},"", EndingColUI::Non)},
        {EndingColUI::Second , UIObj<EndingColUI>("ending_second",{{0,0},{0,0}},"", EndingColUI::Non)},
        {EndingColUI::Third , UIObj<EndingColUI>("ending_third",{{0,0},{0,0}},"", EndingColUI::Non)},
        {EndingColUI::Forth , UIObj<EndingColUI>("ending_forth",{{0,0},{0,0}},"", EndingColUI::Non)},
        {EndingColUI::Five , UIObj<EndingColUI>("ending_five",{{0,0},{0,0}},"", EndingColUI::Non)},
        {EndingColUI::Six , UIObj<EndingColUI>("ending_six",{{0,0},{0,0}},"", EndingColUI::Non)},
     //   {EndingColUI::Seven , UIObj<EndingColUI>("ending_seven",{{0,0},{0,0}},"", EndingColUI::Non)},
    };
    //tmxから取ってきた値をUIMapに代入する
    for (auto& coll : tmxObj_.GetOpeningCollList())
    {
        for (auto& UI : endingUIMap_)
        {
            if (coll.first == UI.second.drawImage)
            {
                //画像の座標にtmxから取ってきた座標を入れる
                UI.second.coll = { coll.second.first,coll.second.second };
            }
        }
        if (coll.first == "operation")
        {
            //ヒントように座標を取っておく
            tipsColl_ = coll.second;
        }
    }
}
