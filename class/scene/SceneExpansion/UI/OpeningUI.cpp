#include "OpeningUI.h"
#include "../../../common/manager/ImageMng.h"
#include "../../SceneManager.h"
#include "../../../input/KeyInput.h"
#include "../../../input/PadInput.h"
#include "../../../tmx/TmxObj.h"

constexpr double WAIT_TIME = 5.0;

OpeningUI::OpeningUI()
{
	Init();
}

OpeningUI::OpeningUI(CntType cntType)
{
    cntType_ = cntType;

    switch (cntType_)
    {
    case CntType::Non:
        break;
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
    case CntType::Max:
        break;
    default:
        break;
    }
	Init();
}

OpeningUI::~OpeningUI()
{
}

void OpeningUI::Init()
{
	InitUIMap();
    time_ = 0.0;
    progress_ = (int)OpeningColUI::Non;
	isEndFlg = false;
}

void OpeningUI::Update()
{
    if (progress_ != (int)OpeningColUI::Non)
    {
        controller_->Update();
        if (controller_->CheckInputKey(KeyID::Decide) || time_ > WAIT_TIME)
        {
            if (progress_ < (int)OpeningColUI::Max - 1)
            {
                progress_++;
                time_ = 0;

            }
            else
            {
                isEndFlg = true;
            }
        }
    }
    else
    {
        if ( time_ > WAIT_TIME)
        {
            if (progress_ < (int)OpeningColUI::Max - 1)
            {
                progress_++;
                time_ = 0;

            }
            else
            {
                isEndFlg = true;
            }
        }
    }
    time_ += lpSceneMng.GetDeltaTime();
}

void OpeningUI::Draw()
{
    
    for (auto UIObj : openingUIMap_)
    {
        if ((OpeningColUI)progress_ == UIObj.first)
        {
            SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(std::sin(DX_PI * 2 / 10 * time_) * 255));
            CollDrawCenter(UIObj.second.coll, UIObj.second.drawImage);
            SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 100);
        }
    }
   // DrawFormatString(0, 0, 0xffffff, "progress:%d", progress_);
}

bool OpeningUI::GetIsEndFlg()
{
	return isEndFlg;
}

int OpeningUI::Getprogress()
{
    return progress_;
}

void OpeningUI::InitUIMap()
{
    //tmx
    tmxObj_.LoadTmx("resource/tmx/opening.tmx", false);
    openingUIMap_ =
    {
        //enum                               画像,        画像の座標 ,tipsの画像,         当たった時に返すenum
        {OpeningColUI::Non, UIObj<OpeningColUI>("opening_non", {{0,0},{0,0}},"", OpeningColUI::Non)},
        {OpeningColUI::First , UIObj<OpeningColUI>("opening_first",{{0,0},{0,0}},"", OpeningColUI::Non)},
        {OpeningColUI::Second , UIObj<OpeningColUI>("opening_second",{{0,0},{0,0}},"", OpeningColUI::Non)},
        {OpeningColUI::Third , UIObj<OpeningColUI>("opening_third",{{0,0},{0,0}},"", OpeningColUI::Non)},
        {OpeningColUI::Forth , UIObj<OpeningColUI>("opening_forth",{{0,0},{0,0}},"", OpeningColUI::Non)},
        {OpeningColUI::Five , UIObj<OpeningColUI>("opening_five",{{0,0},{0,0}},"", OpeningColUI::Non)},
        {OpeningColUI::Six , UIObj<OpeningColUI>("opening_six",{{0,0},{0,0}},"", OpeningColUI::Non)},
        {OpeningColUI::Seven , UIObj<OpeningColUI>("opening_seven",{{0,0},{0,0}},"", OpeningColUI::Non)},
    };
    //tmxから取ってきた値をUIMapに代入する
    for (auto& coll : tmxObj_.GetOpeningCollList())
    {
        for (auto& UI : openingUIMap_)
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
