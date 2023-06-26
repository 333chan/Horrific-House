#include "ExitUI.h"
#include "../../../common/SceneID.h"
#include "../../SceneManager.h"
#include "../../../input/KeyInput.h"
#include "../../../input/PadInput.h"

ExitUI::ExitUI()
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
    //シーンIDが来なかったらMaxをいれる
    sceneID_ = SceneID::Max;
    //連続で入力がされないように更新
    controller_->Update();
    //初期化
    Init();
}

ExitUI::ExitUI(SceneID sceneID)
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
    //前のシーンID
    sceneID_ = sceneID;
    //連続で入力がされないように更新
    controller_->Update();
    //初期化
    Init();
}

ExitUI::~ExitUI()
{
}

void ExitUI::Init()
{
    //tmx読み込み
    tmxObj_.LoadTmx("resource/tmx/exit.tmx", false);
    //Map初期化
    InitUIMap();
}

void ExitUI::Update()
{
    //マウス取得
    GetMousePoint(&mousePos_.x, &mousePos_.y);
    //コントローラの更新
    controller_->Update();
    //クリックして当たったenum格納
    PutClickHit();
    //マウスが当たったら
    mouseHitCol_ = PutMouseHit(tmxObj_.GetExitCollList());
    //フェードインアウトを更新
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
                //選択した枠描画
                SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(std::sin(DX_PI * 2 / 60 *(80 * FadeInOutCnt_)) * 100 + 50));
                DrawExtendGraph(static_cast<int>(UIObj.second.coll.first.x) + 50,
                    static_cast<int>(UIObj.second.coll.first.y),
                    static_cast<int>(UIObj.second.coll.first.x) + static_cast<int>(UIObj.second.coll.second.x) - 50,
                    static_cast<int>(UIObj.second.coll.first.y) + static_cast<int>(UIObj.second.coll.second.y), lpImageMng.GetID("selection2")[0],
                    true);
                SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 100);
            }
        }
        //文字の描画
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
        //オプションだった場合
        exitUIMap_ =
        {
            //enum                               画像,        画像の座標 ,tipsの画像,         当たった時に返すenum
            {ExitColUI::Title, UIObj<ExitColUI>("ExitToTitle",  {{0,0},{0,0}},"",         ExitColUI::Non)},
            {ExitColUI::YesOption, UIObj<ExitColUI>("Yes",  {{0,0},{0,0}},"",           ExitColUI::YesOption)},
            {ExitColUI::NoOption,  UIObj<ExitColUI>("No",   {{0,0},{0,0}},"",         ExitColUI::NoOption) }
        };
    }
    else
    {
        //それ以外だったら
        exitUIMap_ =
        {
            //enum                               画像,        画像の座標 ,tipsの画像,         当たった時に返すenum
            {ExitColUI::Title, UIObj<ExitColUI>("ExitTitle",  {{0,0},{0,0}},"",         ExitColUI::Non)},
            {ExitColUI::Yes, UIObj<ExitColUI>("Yes",  {{0,0},{0,0}},"",           ExitColUI::Yes)},
            {ExitColUI::No,  UIObj<ExitColUI>("No",   {{0,0},{0,0}},"",         ExitColUI::No) }
        };
    }
    //tmxから取ってきた値をUIMapに代入する
    for (auto& coll : tmxObj_.GetExitCollList())
    {
        for (auto& UI : exitUIMap_)
        {
            if (coll.first == UI.second.drawImage)
            {
                //画像の座標にtmxから取ってきた座標を入れる
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
                //選択したenumを格納
                decideHitCol_ = coll.second.CanHitUI;
            }
        }
    }
}