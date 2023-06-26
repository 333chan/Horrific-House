#include "../../SceneManager.h"
#include "../../../common/manager/ImageMng.h"
#include "../../../common/manager/ModelMng.h"
#include "../../../input/KeyInput.h"
#include "../../../input/PadInput.h"
#include "../../../common/CheckHit.h"
#include "../../../common/OriginalShader.h"
#include "../../../tmx/TmxObj.h"
#include "TitleUI.h"

TitleUI::TitleUI()
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
TitleUI::TitleUI(std::unique_ptr<Controller> controller)
{
    //コントローラーの初期化
    controller_ = std::move(controller);
    //コントローラの更新
    controller_->Update();
    //初期化
    Init();
}

TitleUI::~TitleUI()
{
}

void TitleUI::Init()
{
    //tmx
    tmxObj_.LoadTmx("resource/tmx/title.tmx", false);
    //バイナリデータ読み込み
    LoadBinData("resource/data/UI/TitleUIData.dat", "UIObj");
    modelPath_ = "resource/model/UI/";
    //初期化
    for (auto& uiObj : uiObjList_)
    {
        //配置
        MV1SetPosition(lpModelMng.GetID(modelPath_ + uiObj.objName_ + ".mv1")[0], uiObj.pos_.toVECTOR());
        //度数からラジアンに変換
        uiObj.angle_ = { Deg2RadF(uiObj.angle_.x),Deg2RadF(uiObj.angle_.y),Deg2RadF(uiObj.angle_.z) };
        //回転
        MV1SetRotationXYZ(lpModelMng.GetID(modelPath_ + uiObj.objName_ + ".mv1")[0], uiObj.angle_.toVECTOR());
        //拡大値
        MV1SetScale(lpModelMng.GetID(modelPath_ + uiObj.objName_ + ".mv1")[0], uiObj.scale_.toVECTOR());
    }

    InitUIMap();
    //アウトライン描画のシェーダー初期化
    int outL_vs = LoadVertexShader("./resource/shader/vs/OutLine_vs.vso");
    int outL_ps = LoadPixelShader("./resource/shader/ps/OutLine_ps.pso");
    origShader_ = std::make_unique<OriginalShader>(outL_vs, outL_ps);
    //アウトラインなしの普通の描画用シェーダー
    psLight_ = LoadPixelShader("resource/shader/ps/TitleModel_ps.pso");
    vsLight_ = LoadVertexShader("resource/shader/vs/NormalModel_vs.vso");
}

void TitleUI::Update()
{
    //マウス取得
    GetMousePoint(&mousePos_.x, &mousePos_.y);
    //コントローラの更新
   controller_->Update();
   //クリックして当たったenum格納
    PutClickHit();
    //当たった文字列を受け取る
    mouseHitCol_ = PutMouseHit(tmxObj_.GetTitleCollList());
    FadeInOutCnt_ += lpSceneMng.GetDeltaTime();
}

void TitleUI::Draw()
{
    FadeInOutCnt_ += 2;
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(FadeInOutCnt_));
    for (auto& uiObj : titleUIMap_)
    {
        if (uiObj.first == TitleColUI::Mouse)
        {
            //右したのマウス描画
            CollDrawCenterX(uiObj.second.coll, uiObj.second.drawImage);
        }
        //３Dモデルの文字描画
        origShader_->Draw(lpModelMng.GetID(uiObj.second.modelStr)[0], mouseHitCol_ == uiObj.second.drawImage,psLight_,vsLight_);
    }
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void TitleUI::InitUIMap()
{
    //タイトル格納
    titleUIMap_ =
    {
        //enum                               画像,        画像の座標 ,tipsの画像,         当たった時に返すenum
        {TitleColUI::Play,      UIObjWithModel<TitleColUI>("play",       {{0,0},{0,0}},"gamePlay",         TitleColUI::Play,    "resource/model/UI/Play.mv1")},
        {TitleColUI::Option,    UIObjWithModel<TitleColUI>("option",     {{0,0},{0,0}},"optionOpen",       TitleColUI::Option,  "resource/model/UI/Option.mv1")},
        {TitleColUI::Exit,      UIObjWithModel<TitleColUI>("exit",       {{0,0},{0,0}},"returnToDesktop",  TitleColUI::Exit,    "resource/model/UI/Exit.mv1")},
        {TitleColUI::Mouse,     UIObjWithModel<TitleColUI>("mouse",      {{0,0},{0,0}},"",                 TitleColUI::Non,     "")},
        {TitleColUI::Operation, UIObjWithModel<TitleColUI>("operation",  {{0,0},{0,0}},"",                 TitleColUI::Non,     "")}
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
        if (coll.first == "operation")
        {
            //ヒントように座標を取っておく
            tipsColl_ = coll.second;
        }
    }
}

void TitleUI::PutClickHit()
{
    decideHitCol_ = TitleColUI::Non;
    if (controller_->CheckInputKey(KeyID::Decide))
    {
        for (auto& coll : titleUIMap_)
        {
            if (CheckCollMouse()(mousePos_, coll.second.coll))
            {
                //クリックした時当たったenumをいれる
                decideHitCol_ = coll.second.CanHitUI;
            }
        }
    }
}

void TitleUI::LoadBinData(const char* bData, std::string pName)
{
    int file = FileRead_open(bData);
    int version;
    FileRead_read(&version, sizeof(version), file);
    if (version == 6)
    {
        while (!FileRead_eof(file))
        {
            std::string name;	//名前
            Vector3 pos;	//座標
            Vector3 rot;	//回転
            Vector3 scale;	//回転
            char nameSize;
            //ハンドル、名前、座標、回転情報読み取り
            FileRead_read(&nameSize, sizeof(nameSize), file);
            name.resize(static_cast<int>(nameSize));
            FileRead_read(&name[0], nameSize, file);
            FileRead_read(&pos, sizeof(pos), file);
            FileRead_read(&rot, sizeof(rot), file);
            FileRead_read(&scale, sizeof(scale), file);
            if (pName == name)
            {
            	continue;
            }
            //名前、座標、回転情報を格納
            uiObjList_.emplace_back(name, pos, rot,scale);
        }
    }
    //ファイルを閉じる
    FileRead_close(file);
}

TitleColUI TitleUI::GetHitUICol()
{
    return decideHitCol_;
}

ControllerType TitleUI::GetControllerType()
{
    return controller_->GetControllerType();
}

std::unique_ptr<Controller> TitleUI::GetController()
{
    return std::move(controller_);
}


bool TitleUI::GetPreEndFlag()
{
    return PreEndFlag_;
}


