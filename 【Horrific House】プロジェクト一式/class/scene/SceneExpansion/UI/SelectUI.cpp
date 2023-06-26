#include "SelectUI.h"

#include "../../../input/KeyInput.h"
#include "../../../input/PadInput.h"
#include "../../../tmx/TmxObj.h"
#include "../../../common/CheckHit.h"
#include "../../SceneManager.h"
#include "../../../common/manager/ImageMng.h"

SelectUI::SelectUI()
{
    //パッドの数が１つ以上の場合はパッド操作にする
    if (GetJoypadNum() > 0)
    {
        controller_ = std::make_unique<PadInput>();
    }
    else
    {
        controller_ = std::make_unique<KeyInput>();
    }
    //コントローラの更新
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
    //マウス取得
    GetMousePoint(&mousePos_.x, &mousePos_.y);
    //コントローラの更新
    controller_->Update();
    //クリックして当たったenum格納
    PutClickHit();
    //当たった文字列を受け取る
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
                //マウスに当たっている所に枠描画
                SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(std::sin(DX_PI * 2 / 60 * (111 * FadeInOutCnt_)) * 100 + 50));                
                DrawExtendGraph(static_cast<int>(UIObj.second.coll.first.x),
                    static_cast<int>(UIObj.second.coll.first.y),
                    static_cast<int>(UIObj.second.coll.first.x) + static_cast<int>(UIObj.second.coll.second.x),
                    static_cast<int>(UIObj.second.coll.first.y) + static_cast<int>(UIObj.second.coll.second.y), lpImageMng.GetID("selection1")[0],
                    true);
                SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 100);
            }
            //ヒントの描画
            CollDrawCenterX(tipsColl_, UIObj.second.tipsImage);
        }
        if (cntType_ == UIObj.second.type && cntType_ != CntType::Non)
        {
            //デバイスを選択した枠描画
            SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(std::sin(DX_PI * 2 / 60 * (30 * FadeInOutCnt_)) * 100 +100));
            DrawExtendGraph(static_cast<int>(UIObj.second.coll.first.x),
            static_cast<int>(UIObj.second.coll.first.y - 20),
                static_cast<int>(UIObj.second.coll.first.x) + static_cast<int>(UIObj.second.coll.second.x),
                static_cast<int>(UIObj.second.coll.first.y) + static_cast<int>(UIObj.second.coll.second.y), lpImageMng.GetID("selection")[0],
                true);
            SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 100);
        }
        //デバイスが画像描画
        DrawExtendGraph(static_cast<int>(UIObj.second.coll.first.x + 50),
            static_cast<int>(UIObj.second.coll.first.y+70),
            static_cast<int>(UIObj.second.coll.first.x) + static_cast<int>(UIObj.second.coll.second.x) - 50,
            static_cast<int>(UIObj.second.coll.first.y) + static_cast<int>(UIObj.second.coll.second.y)-20, lpImageMng.GetID(UIObj.second.deviceImage)[0],
            true);
        //文字の描画
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
        //enum                               画像,        画像の座標 ,tipsの画像,         当たった時に返すenum
        {SelectColUI::Title, UISelectObj<SelectColUI>(  "select_title", {{0,0},{0,0}},"select_title_tip", "",               CntType::Non,                 SelectColUI::Non)},
        {SelectColUI::Key, UISelectObj<SelectColUI>(    "select_key",   {{0,0},{0,0}},"select_key_tip",   "desktopPc_Image",CntType::Key,                 SelectColUI::Non)},
        {SelectColUI::Pad, UISelectObj<SelectColUI>(    "select_pad",   {{0,0},{0,0}},"select_pad_tip",   "gamePad_Image",  CntType::Pad,                 SelectColUI::Non)},
        {SelectColUI::Ar , UISelectObj<SelectColUI>(    "select_ar",    {{0,0},{0,0}},"select_ar_tip",    "smh_Image",      CntType::Smh,                 SelectColUI::Non)},
        {SelectColUI::Decide , UISelectObj<SelectColUI>("select_decide",{{0,0},{0,0}},"select_decide_tip","",               CntType::Non,                 SelectColUI::Decide)},
        {SelectColUI::Return , UISelectObj<SelectColUI>("select_return",{{0,0},{0,0}},"select_return_tip","",               CntType::Non,                 SelectColUI::Return)},
    };
    //tmxから取ってきた値をUIMapに代入する
    for (auto& coll : tmxObj_.GetSelectCollList())
    {
        for (auto& UI : selectUIMap_)
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

void SelectUI::PutClickHit()
{
    decideHitCol_ = SelectColUI::Non;
    if (controller_->CheckInputKey(KeyID::Decide))
    {
        for (auto& coll : selectUIMap_)
        {
            if (CheckCollMouse()(mousePos_, coll.second.coll))
            {
                //選択したenumを返す
                decideHitCol_ = coll.second.CanHitUI;
                if (coll.second.type != CntType::Non)
                {
                    //フェードインを初期化
                    FadeInOutCnt_ = 0;
                    //クリックデバイスのタイプを格納
                    cntType_ = coll.second.type;
                }
            }
        }
    }
}
