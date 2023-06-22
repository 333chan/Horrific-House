#include <math.h>
#include "../../SceneManager.h"
#include "../../../common/manager/SoundMng.h"
#include "../../../common/manager/CBufferMng.h"
#include "../../../tmx/TmxObj.h"
#include "../../../input/PadInput.h"
#include "../../../input/KeyInput.h"
#include "../../../common/OriginalShader.h"
#include "../../../common/manager/ModelMng.h"
#include "OptionUI.h"

constexpr int DRAW_OFFSET = 50;
constexpr float LIGHT_MAGNIFICATION = 1200.0f;
OptionUI::OptionUI(SceneID beforeSceneID)
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
    controller_->Update();
    beforeSceneID_ = beforeSceneID;
    Init();
}

OptionUI::~OptionUI()
{
}

void OptionUI::Init()
{
    //描画用シェーダー初期化
    origShader_ = std::make_unique<OriginalShader>(
        LoadVertexShader("resource/shader/vs/NormalModel_vs.vso"),
        LoadPixelShader("resource/shader/ps/GameModel_ps.pso"));
    //モデルの大きさ変更
    MV1SetScale(lpModelMng.GetID("resource/model/marker/redMarker.mv1")[0], { 1.5,1.5,1.5 });
    MV1SetScale(lpModelMng.GetID("resource/model/marker/greenMarker.mv1")[0], { 1.5,1.5,1.5 });
    //モデルの座標セット
    MV1SetPosition(lpModelMng.GetID("resource/model/marker/redMarker.mv1")[0], { 1500,0,0 });
    MV1SetPosition(lpModelMng.GetID("resource/model/marker/greenMarker.mv1")[0], { 3000,0,0 });
    //tmxロード
    tmxObj_.LoadTmx("resource/tmx/option.tmx", false);
    //それぞれのマップに値を格納
    displayMap_ =
    {
        {"optionTag","optionTag"},
    };
    controlMap_ =
    {
        {"key","key"},
        {"keyboard","keyboard"},
        {"smp","smp"},
        {"smartphone","SumahoImage"},
        {"pad","pad"},
        {"padImage","padImage"}
    };
    audioMap_ =
    {
        {"volume","volume"},
        {"masterValume","masterValume"},
        {"valumeControl","select5"},
        {"non","select5"}
    };

    graphicMap_ =
    {
        {"brightness","screenBrightness"},
        {"screenBrightness","brightness"},
        {"brightnessAdjustment","select5"},
    };
    titleRetureMap_ =
    {
        {"",""}
    };
    retureMap_ =
    {
        {"",""}
    };
    selectMap_ =
    {
        {"controlTag",&OptionUI::UpdateControl},
        {"audioTag",&OptionUI::UpdateAudio },
        {"graphicTag",&OptionUI::UpdateGraphic},
        {"titleReturnTag",&OptionUI::UpdateToTitle},
        {"returnTag",&OptionUI::UpdateReturnScreen}
    };

    supportMap_ =
    {
        {"controlTag","supportControl"},
        {"audioTag","supportAudio"},
        {"graphicTag","supportGraphic"},
        {"titleReturnTag","supportTitleReturn"},
        {"returnTag","supportReturn"},
    };


    Volume_ = lpSoundMng.GetAllVolume();
    if (Volume_ == 0)
    {
        Volume_ = 125;
    }
    brightness_ = lpCBufferMng.GetALLambientLight().x* LIGHT_MAGNIFICATION;
    FadeInOutCnt_ = 0;

    afterSceneID_ = OptionColUI::Non;
}

void OptionUI::Update()
{

    //時間の更新
    FadeInOutCnt_ += lpSceneMng.GetDeltaTime();
    //コントローラ
    controller_->Update();

    //衝突判定と押したボタンの代入
    HitCollAndAssignProcess();
    if (controller_->CheckInputKey(KeyID::Pause))
    {
        afterSceneID_ = OptionColUI::Reture;
    }
    //関数ポインタの実装
    if (UpdateFuncPtr_ != nullptr)
    {
        (this->*UpdateFuncPtr_)();
    }
    
    //明るさの更新
    //lpSceneMng.SetBrightnessScrren(brightness_);
    lpCBufferMng.SetALLambientLight({ brightness_ / LIGHT_MAGNIFICATION, brightness_ / LIGHT_MAGNIFICATION, brightness_ / LIGHT_MAGNIFICATION });

}

void OptionUI::Draw()
{
    DrawExtendGraph(0, 0, lpSceneMng.GetScreenSize().x, lpSceneMng.GetScreenSize().y, lpImageMng.GetID("bg")[0], false);

    //tmxからのコリジョンの場所に描画
    DrawColl();
#ifdef _DEBUG
    //test_ = lpCBufferMng.GetALLambientLight().x;
    DrawFormatString(0, 450, 0xffff00, "明るさ　　%f", lpCBufferMng.GetALLambientLight().x);
    DrawFormatString(0, 500, 0xffff00, "音量　　%d", Volume_);
    DrawFormatString(0, 550, 0xffff00, "get音量　　%d", lpSoundMng.GetAllVolume());
    
#endif
}

OptionColUI OptionUI::GetAfterSceneCol()
{
    return afterSceneID_;
}

void OptionUI::HitCollAndAssignProcess()
{
    Vector2Int pos;
    GetMousePoint(&pos.x, &pos.y);
    for (auto& coll : tmxObj_.GetOptionCollList())
    {
        if (CheckCollMouse()(pos, coll.second))
        {
            if (controller_->CheckInputKey(KeyID::Decide))
            {
                //クリックしたときに当たっていたら関数ポインタに処理を入れる
                HitCollClickAndAssignProcess(coll);
            }
            //マウスが当たっていたら当たっている画像のstringを代入する
            OldhitMouse_ = hitMouse_;
            hitMouse_ = HitCollMouse(coll);
            //当たったのが変わったときに表示カウントをリセットする
            if (OldhitMouse_ != hitMouse_)
            {
                FadeInOutCnt_ = 0;
            }
        }
    }
}

void OptionUI::HitCollClickAndAssignProcess(std::pair<std::string, Collision> coll)
{
    //クリックしたときに当たっているか
    for (auto& select : selectMap_)
    {
        if (coll.first == select.first)
        {
            //選択したリストをドローするためのマップに格納している
            selection_ = select.first;
            UpdateFuncPtr_ = select.second;
        }
    }
}

//マウスが当たっているか
std::pair<std::string, std::string> OptionUI::HitCollMouse(std::pair<std::string, Collision> coll)
{
    for (auto& select : supportMap_)
    {
        if (coll.first == select.first)
        {
            //マウスが当たったら
            return { select.first,select.second };
        }
       
    }
    return { "","" };
}

void OptionUI::UpdateControl()
{
    DrawFuncPtr_ = &OptionUI::DrawControl;
}

void OptionUI::UpdateAudio()
{
    Vector2Int pos;
    bool Decideflag_  = false;
    GetMousePoint(&pos.x, &pos.y);
    for (auto& coll : tmxObj_.GetOptionCollList())
    {
        if (CheckCollMouse()(pos, coll.second))
        {
            if (!controller_->CheckLongInputKey(KeyID::Decide))
            {
                continue;
            }
            for (auto& UpdateAudio : audioMap_)
            {
                if (coll.first != UpdateAudio.first)
                {
                    continue;
                }
                //音量の調整
                Volume_ = pos.x - static_cast<int>(coll.second.first.x);
                Decideflag_ = true;
            }

        }
    }
    if (Decideflag_)
    {
        //音量の設定
        lpSoundMng.AllChangeVolume(Volume_ / 2);
        lpSoundMng.SetAllVolume(Volume_ / 2);
        lpSoundMng.PlayingSound("./resource/sound/playerWalkSE.mp3");
    }
    lpSoundMng.ShortCheckStopSound("./resource/sound/playerWalkSE.mp3",1);
    DrawFuncPtr_ = &OptionUI::DrawAudio;
}

void OptionUI::UpdateGraphic()
{
    Vector2Int pos;
    GetMousePoint(&pos.x, &pos.y);
    for (auto& coll : tmxObj_.GetOptionCollList())
    {
        if (CheckCollMouse()(pos, coll.second))
        {
            if (!controller_->CheckLongInputKey(KeyID::Decide))
            {
                continue;
            }
            for (auto& graphic : graphicMap_)
            {
                if (coll.first != graphic.first)
                {
                    continue;
                }
                //明るさを調整
                brightness_ = pos.x - coll.second.first.x;
            }

        }
    }
    DrawFuncPtr_ = &OptionUI::DrawGraphic;
}

void OptionUI::UpdateToTitle()
{
    if (SceneID::Game == beforeSceneID_)
    {
        afterSceneID_ = OptionColUI::TitleReture;
    }
    DrawFuncPtr_ = &OptionUI::DrawToTitle;
}

void OptionUI::UpdateReturnScreen()
{
    afterSceneID_ = OptionColUI::Reture;
    DrawFuncPtr_ = &OptionUI::DrawReturnScreen;
}

void OptionUI::DrawControl()
{
    for (auto& coll : tmxObj_.GetOptionCollList())
    {
        DrawList(coll, controlMap_);
    }
}

void OptionUI::DrawAudio()
{
    for (auto& coll : tmxObj_.GetOptionCollList())
    {
        for (auto& audio : audioMap_)
        {
            //同じstringじゃなかったらはじく
            if (coll.first == audio.first)
            {

                if (audio.first == "volume")
                {
                    DrawCollCenterY(coll, audio.second);
                }
                else if (audio.first == "masterValume")
                {
                    DrawCollCenterY(coll, audio.second);
                }
                else if (audio.first == "non")
                {
                    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 70);
                    DrawExtendGraph(static_cast<int>(coll.second.first.x) - DRAW_OFFSET,
                        static_cast<int>(coll.second.first.y),
                        static_cast<int>(coll.second.first.x) + static_cast<int>(coll.second.second.x) + DRAW_OFFSET,
                        static_cast<int>(coll.second.first.y) + static_cast<int>(coll.second.second.y), lpImageMng.GetID(audio.second)[0],
                        true);
                    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 125);

                }
                else if (audio.first == "valumeControl")
                {
                    SetDrawBlendMode(DX_BLENDMODE_ADD, 230);
                    DrawExtendGraph(static_cast<int>(coll.second.first.x),
                        static_cast<int>(coll.second.first.y) - 10,
                        static_cast<int>(coll.second.first.x) + static_cast<int>(coll.second.second.x),
                        static_cast<int>(coll.second.first.y) + static_cast<int>(coll.second.second.y) + 10, lpImageMng.GetID(audio.second)[0],
                        true);
                    DrawExtendGraph(static_cast<int>(coll.second.first.x) + 15,
                        static_cast<int>(coll.second.first.y) - 10,
                        static_cast<int>(coll.second.first.x) + Volume_ + 20,
                        static_cast<int>(coll.second.first.y) + static_cast<int>(coll.second.second.y) + 10, lpImageMng.GetID("Bbox")[0],
                        true);
                    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 125);
                }
                continue;
            }
        }
    }
}

void OptionUI::DrawGraphic()
{
    MV1DrawModel(lpModelMng.GetID("resource/model/marker/redMarker.mv1")[0]);
    origShader_->Draw(lpModelMng.GetID("resource/model/marker/greenMarker.mv1")[0]);

    for (auto& coll : tmxObj_.GetOptionCollList())
    {
        for (auto& audio : graphicMap_)
        {
            //同じstringじゃなかったらはじく
            if (coll.first == audio.first)
            {
                if (audio.first == "brightness")
                {
                    DrawCollCenterY(coll, audio.second);
                }
                else if (audio.first == "screenBrightness")
                {
                    DrawCollCenterY(coll, audio.second);
                }
                else if (audio.first == "brightnessAdjustment")
                {
                    SetDrawBlendMode(DX_BLENDMODE_ADD, 230);
                    DrawExtendGraph(static_cast<int>(coll.second.first.x),
                        static_cast<int>(coll.second.first.y) - 10,
                        static_cast<int>(coll.second.first.x) + static_cast<int>(coll.second.second.x),
                            static_cast<int>(coll.second.first.y) + static_cast<int>(coll.second.second.y) + 10, lpImageMng.GetID(audio.second)[0],
                        true);
                    DrawExtendGraph(static_cast<int>(coll.second.first.x) + 15,
                        static_cast<int>(coll.second.first.y) - 10,
                        static_cast<int>(coll.second.first.x + brightness_ + 20),
                        static_cast<int>(coll.second.first.y) + static_cast<int>(coll.second.second.y) + 10, lpImageMng.GetID("Bbox")[0],
                        true);
                    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 125);
                }
                continue;
            }
        }
    }
}

void OptionUI::DrawToTitle()
{
    for (auto& coll : tmxObj_.GetOptionCollList())
    {
        DrawList(coll, titleRetureMap_);
    }
}

void OptionUI::DrawReturnScreen()
{
    for (auto& coll : tmxObj_.GetOptionCollList())
    {
        DrawList(coll, retureMap_);
    }
}

void OptionUI::DrawColl()
{
    for (auto& coll : tmxObj_.GetOptionCollList())
    {
        //表示するだけの描画
        DrawList(coll, displayMap_);

        //selectListだけの描画
        DrawSelectList(coll);

        //関数ポインタの実装
        if (DrawFuncPtr_ != nullptr)
        {
            (this->*DrawFuncPtr_)();
        }
    }
}

void OptionUI::DrawCollCenterY(std::pair<std::string, Collision> coll, std::string imageStr)
{
    Vector2Int GraphSize;
    Vector2 cen = { coll.second.first.x + coll.second.second.x / 2 ,coll.second.first.y + coll.second.second.y / 2 };

    GetGraphSize(lpImageMng.GetID(imageStr)[0], &GraphSize.x, &GraphSize.y);
    GraphSize.y /= 2;
    DrawGraph(static_cast<int>(coll.second.first.x), cen.y - GraphSize.y, lpImageMng.GetID(imageStr)[0], true);
}

void OptionUI::DrawCollCenter(std::pair<std::string, Collision> coll, std::string imageStr)
{
    Vector2Int GraphSize;
    Vector2 cen = { coll.second.first.x + coll.second.second.x / 2 ,coll.second.first.y + coll.second.second.y / 2 };

    GetGraphSize(lpImageMng.GetID(imageStr)[0], &GraphSize.x, &GraphSize.y);
    GraphSize.x /= 2;
    GraphSize.y /= 2;
    DrawGraph(static_cast<int>(cen.x) - GraphSize.x, static_cast<int>(cen.y) - GraphSize.y, lpImageMng.GetID(imageStr)[0], true);
}

void OptionUI::DrawSelectList(std::pair<std::string, Collision> coll)
{
    //titleReturnTagだけゲームシーンのみで出したいのでゲーム以外だったら返す
    if (coll.first == "titleReturnTag")
    {
        if (beforeSceneID_ != SceneID::Game)
        {
            return;
        }
    }

    //項目に触れているものを描画
    if (coll.first == hitMouse_.first)
    {
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(std::sin(DX_PI * 2 / 60 * (80 * FadeInOutCnt_)) * 300 + 100));
        DrawExtendGraph(static_cast<int>(coll.second.first.x) - DRAW_OFFSET,
            static_cast<int>(coll.second.first.y),
            static_cast<int>(coll.second.first.x) + static_cast<int>(coll.second.second.x) - DRAW_OFFSET,
            static_cast<int>(coll.second.first.y) + static_cast<int>(coll.second.second.y), lpImageMng.GetID("selection1")[0],
            true);
        SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
    }
    if (coll.first == "operation")
    {
        if (hitMouse_.first == "titleReturnTag")
        {
            if (beforeSceneID_ != SceneID::Game)
            {
                return;
            }
        }
        DrawCollCenter(coll, hitMouse_.second);
    }
    //項目をクリックしているものを描画
    if (coll.first == selection_)
    {
        DrawExtendGraph(static_cast<int>(coll.second.first.x) - DRAW_OFFSET,
            static_cast<int>(coll.second.first.y),
            static_cast<int>(coll.second.first.x) + static_cast<int>(coll.second.second.x) - DRAW_OFFSET,
            static_cast<int>(coll.second.first.y) + static_cast<int>(coll.second.second.y), lpImageMng.GetID("selection")[0],
            true);
    }

    //選択項目の描画
    for (auto& select : selectMap_)
    {
        //同じものじゃなかったら弾く
        if (coll.first != select.first)
        {
            continue;
        }

        //描画
        DrawCollCenterY(coll, select.first);
        continue;

    }
}

void OptionUI::DrawList(std::pair<std::string, Collision> coll, std::map<std::string, std::string>maplist)
{
    for (auto& draw : maplist)
    {
        //同じstringじゃなかったらはじく
        if (coll.first != draw.first)
        {
            continue;
        }
        DrawCollCenterY(coll, draw.second);
        continue;
    }
}
