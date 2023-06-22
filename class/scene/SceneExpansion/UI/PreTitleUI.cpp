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
    //パッドの数が１つ以上の場合はパッド操作にする
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
    //マウス取得
    GetMousePoint(&mousePos_.x, &mousePos_.y);
    //コントローラの更新
    controller_->Update();
    //クリックして当たったenum格納
    PutClickHit();
    FadeInOutCnt_++;
}

void PreTitleUI::Draw()
{
    for (auto UIObj : titleUIMap_)
    {
        if (UIObj.first == TitleColUI::Title)
        {
            //タイトルだけ描画
            CollDrawCenterX(UIObj.second.coll, UIObj.second.drawImage);
        }
    }
    //中心に描画
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
        //enum                               画像,        画像の座標 ,tipsの画像,         当たった時に返すenum
        {TitleColUI::Title, UIObj<TitleColUI>("title",  {{0,0},{0,0}},"",                 TitleColUI::Non)},
    };
    //tmxから取ってきた値をUIMapに代入する
    for (auto& coll : tmxObj_.GetTitleCollList())
    {
        for (auto& UI : titleUIMap_)
        {
            if (coll.first == UI.second.drawImage)
            {
                //画像の座標にtmxから取ってきた座標を入れる
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
