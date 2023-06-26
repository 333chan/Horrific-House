#include<DxLib.h>
#include "InventoryScene.h"
#include "SceneManager.h"
#include "../common/manager/ModelMng.h"
#include "../common/manager/ImageMng.h"
#include "../common/manager/CBufferMng.h"
#include "../common/manager/SoundMng.h"

#include "../object/Item/Item.h"

#include "../input/PadInput.h"
#include "../input/KeyInput.h"

#include "../tmx/TmxObj.h"
#include "../common/CheckHit.h"
#include "../common/Geometry.h"
#include "../common/MouseIcon.h"

#include"../common/OriginalShader.h"
#include "../common/SocketServer.h"

#include "../../_debug/_DebugConOut.h"

//ぼかしの度合い
constexpr float BLUR_SIGMA = 8.0f;

//↓何のオフセットか説明求
constexpr float OFFSET = 50.0f;

//写真にマウスを合わせたときに追加する値
constexpr float PHOTO_ADD_POS = 5.0f;

//拡大時の位置、大きさ
constexpr float PHOTO_EXPANSION_SIZE_X = 1200.0f;
constexpr float PHOTO_EXPANSION_SIZE_Y = PHOTO_EXPANSION_SIZE_X * 0.5655;
constexpr float PHOTO_EXPANSION_POS_X = (1920.0f- PHOTO_EXPANSION_SIZE_X) / 2.0f;
constexpr float PHOTO_EXPANSION_POS_Y = (1080.0f - PHOTO_EXPANSION_SIZE_Y) / 2.0f;

//フレームの色(仮)
constexpr int   FRAME_COLOR = 125;

InventoryScene::InventoryScene(UniqueScene beforeScene,int bgHandle,ControllerType type, const std::vector<InventoryObj>& inventoryList, const std::vector<int>& photo)
{
    beforeScene_ = std::move(beforeScene);

    if (type == ControllerType::Pad)
    {
        controller_ = std::make_unique<PadInput>();
    }
    else
    {
        controller_ = std::make_unique<KeyInput>();
    }

    //背景
    backGroundHandle_ = bgHandle;
    controller_->Update();

    inventoryList_ = inventoryList;

    //写真初期化
    std::string name;
    for (int i = 0; i < photo.size(); i++)
    {
        name = "photo" + std::to_string(i + 1);
        photoTbl_.try_emplace(name, photo[i]);
    }

    Init();
    test_ = 0;
    //ちらつき防止
    SetDrawScreen(screenID_);
    ClsDrawScreen();
    DrawPrimitive2D(totalBlurVerts_.data(), totalBlurVerts_.size(), DX_PRIMTYPE_TRIANGLESTRIP, backGroundHandle_, false);
}

InventoryScene::~InventoryScene()
{
    Release();
}

void InventoryScene::Init(void)
{
    lpMouseIcon.Init();
    //テーブル初期化
    InitTable();

    //ブラーのピクセルシェーダハンドル
    psBlurXandYHandle_ = LoadPixelShader("./resource/shader/ps/blur_ps.pso");

    //レンダリングターゲット作成
    mainRenderTarget_ = MakeScreen(screenSizeX_, screenSizeY_);
    font_ = CreateFontToHandle("HG教科書体", 17, 3, DX_FONTTYPE_NORMAL);

    //頂点初期化
    InitVerts();

    //重み計算
    CalcBlurWeights();

    //定数バッファ初期化、初期化時に一度更新
    for (int n = 0; n < WEIGHTS_NUM; n++)
    {
        //lpSceneMng.SetWriterData(n, weights_[n]);
        lpCBufferMng.SetPixelCbuffer(9, n, weights_[n]);
    }
    lpCBufferMng.Update();

    InitBlur();

    if (lpSceneMng.GetInventoryCnt() == 1)
    {
        isDrawTips_ = true;
    }

    //写真関係
    psPhotoHandle_  = LoadPixelShader("./resource/shader/ps/Memo_ps.pso");
    photoOrigShader_ = std::make_unique<OriginalShader>(psPhotoHandle_);
    int VertexShader = LoadVertexShader("resource/shader/vs/NormalModel_vs.vso");
    int PixelShader = LoadPixelShader("resource/shader/ps/TexOnlyModel_ps.pso");
    origShader_ = std::make_unique<OriginalShader>(VertexShader, PixelShader);
    InitInventory();
}

void InventoryScene::InitBlur(void)
{
    //横ブラー初期化
    blurX_.Init({ screenSizeX_ / 2, screenSizeY_ }, "./resource/shader/vs/blurX_vs.vso");
    blurX_.origShader = std::make_unique<OriginalShader>(blurX_.vsHandle, psBlurXandYHandle_);

    //縦ブラー初期化
    blurY_.Init({ screenSizeX_, screenSizeY_ / 2 }, "./resource/shader/vs/blurY_vs.vso");
    blurY_.origShader = std::make_unique<OriginalShader>(blurY_.vsHandle, psBlurXandYHandle_);
}

void InventoryScene::InitVerts(void)
{
    for (auto& v : totalBlurVerts_)
    {
        v.rhw = 1.0f;
        v.dif = GetColorU8(255, 255, 255, 255);
        v.pos.z = 0.0f;
    }

    totalBlurVerts_[0].pos.x = 0.0f;
    totalBlurVerts_[0].pos.y = 0.0f;

    totalBlurVerts_[1].pos.x = static_cast<float>(screenSizeX_);
    totalBlurVerts_[1].pos.y = 0.0f;

    totalBlurVerts_[2].pos.x = 0.0f;
    totalBlurVerts_[2].pos.y = static_cast<float>(screenSizeY_);

    totalBlurVerts_[3].pos.x = static_cast<float>(screenSizeX_);
    totalBlurVerts_[3].pos.y = static_cast<float>(screenSizeY_);

    totalBlurVerts_[0].u = 0.0f;
    totalBlurVerts_[0].v = 0.0f;

    totalBlurVerts_[1].u = 1.0f;
    totalBlurVerts_[1].v = 0.0f;

    totalBlurVerts_[2].u = 0.0f;
    totalBlurVerts_[2].v = 1.0f;

    totalBlurVerts_[3].u = 1.0f;
    totalBlurVerts_[3].v = 1.0f;

}

UniqueScene InventoryScene::Update(UniqueScene scene)
{
    controller_->Update();
    
    if (controller_->CheckInputKey(KeyID::Inventory))
    {
        //マウスカーソルを真ん中に戻してカメラ回転をしないようにする
        //真ん中に戻さなかった場合はインベントリから戻った際に動かした分回転してしまう
        SetMousePoint(lpSceneMng.GetScreenSize().x / 2, lpSceneMng.GetScreenSize().y / 2);
        lpSoundMng.PlayingSound("resource/sound/InventoryCloseSE.mp3");
        return std::move(beforeScene_);
    }

    UpdateInventory();
    
    //デバッグ用
    if (CheckHitKey(KEY_INPUT_6))
    {
        isDrawTips_ = false;
    }

    DrawScreen();
    return scene;
}

void InventoryScene::DrawScreen(void)
{
    //メインレンダリングターゲットに描画
    SetDrawScreen(mainRenderTarget_);
    ClsDrawScreen();

    //背景
    DrawPrimitive2D(totalBlurVerts_.data(), totalBlurVerts_.size(), DX_PRIMTYPE_TRIANGLESTRIP, backGroundHandle_, false);

    //横ブラーをかける
    SetDrawScreen(blurX_.renderTarget);
    ClsDrawScreen();
    blurX_.origShader->DrawPrimitive3D({ 0.0f,0.0f }, blurX_.imageSize, mainRenderTarget_);

    //縦ブラーをかける
    SetDrawScreen(blurY_.renderTarget);
    ClsDrawScreen();
    blurY_.origShader->DrawPrimitive3D({ 0.0f,0.0f },blurY_.imageSize, blurX_.renderTarget);

    //最終描画
    SetDrawScreen(screenID_);
    ClsDrawScreen();
    
    //縦ブラーのものを描画
    DrawPrimitive2D(totalBlurVerts_.data(), totalBlurVerts_.size(), DX_PRIMTYPE_TRIANGLESTRIP, blurY_.renderTarget, false);

    DrawInventory();
}

void InventoryScene::Release(void)
{
    DeleteGraph(mainRenderTarget_);
    DeleteGraph(blurX_.renderTarget);
    DeleteGraph(blurY_.renderTarget);

    DeleteShader(psBlurXandYHandle_);
    DeleteShader(blurX_.vsHandle);
    DeleteShader(blurY_.vsHandle);
    DeleteShader(psPhotoHandle_);
}

void InventoryScene::CalcBlurWeights(void)
{
    //重みの合計
    float totalWeight = 0.0f;

    //xは基準テクセルからの距離
    for (int x = 0; x < WEIGHTS_NUM; x++)
    {
        weights_[x] = expf(-0.5f * static_cast<float>(x * x) / BLUR_SIGMA);
        totalWeight += 2.0f * weights_[x];
    }

    //除算し、合計を1にする
    for (int i = 0; i < WEIGHTS_NUM; i++)
    {
        weights_[i] /= totalWeight;
    }
}

void InventoryScene::CollMousePosAndPhoto(void)
{
    //マウスの位置取得
    Vector2Int mPos;
    GetMousePoint(&mPos.x, &mPos.y);

    for (const auto& coll : tmxObj_.GetInventoryCollList())
    {
        std::string collName = coll.first;
        //対応した位置でなければcontinue
        if (!photoTbl_.count(collName))
        {
            continue;
        }

        //マウスの位置と写真の位置が同じかどうか判定
        if (CheckCollMouse()(mPos, coll.second))
        {
            //拡大フラグを立てる
            photoTbl_[collName].addFlag = true;

            if (controller_->CheckInputKey(KeyID::Decide))
            {
                photoTbl_[collName].expansionFlag = true;
                photoTbl_[collName].stepLerpTime = PHOTO_LERP_TIME;
            }
        }
        else
        {
            //他の写真はカーソルがあっていないのでfalseにする
            photoTbl_[collName].addFlag = false;
        }
    }
}

void InventoryScene::DrawPhoto(std::string name, Vector2 pos, Vector2 imageSize)
{
    if (!photoTbl_.count(name))
    {
        return;
    }

    //最終位置計算(ラムダ)
    auto totalPos = [](Vector2 initPos,Vector2 addPos) {
        Vector2 vec2Pos = initPos + addPos;
        Vector3 vec3Pos = { vec2Pos.x,vec2Pos.y,0.0f };
        return vec3Pos;
    };

    //マウスカーソルと重なっていた場合は少し拡大する
    Vector2 addPos = photoTbl_[name].addFlag ? Vector2(PHOTO_ADD_POS, PHOTO_ADD_POS) : Vector2(0.0f, 0.0f);

    //頂点情報
    std::array<Vector3, 4> vertPos;

    vertPos[0] = totalPos({ pos.x,pos.y },                             { -addPos.x,-addPos.y });
    vertPos[1] = totalPos({ pos.x + imageSize.x,pos.y },               {  addPos.x,-addPos.y });
    vertPos[2] = totalPos({ pos.x,pos.y + imageSize.y },               { -addPos.x, addPos.y });
    vertPos[3] = totalPos({ pos.x + imageSize.x,pos.y + imageSize.y }, {  addPos.x, addPos.y });
    //拡大フラグがある場合は補間をする
    if (photoTbl_[name].expansionFlag)
    {
        float lerpTime = 1.0f - (photoTbl_[name].stepLerpTime / PHOTO_LERP_TIME);
        vertPos[0] = Lerp(vertPos[0], { PHOTO_EXPANSION_POS_X,PHOTO_EXPANSION_POS_Y,1.0f }, lerpTime);
        vertPos[1] = Lerp(vertPos[1], { PHOTO_EXPANSION_POS_X + PHOTO_EXPANSION_SIZE_X,PHOTO_EXPANSION_POS_Y,1.0f }, lerpTime);
        vertPos[2] = Lerp(vertPos[2], { PHOTO_EXPANSION_POS_X,PHOTO_EXPANSION_POS_Y + PHOTO_EXPANSION_SIZE_Y,1.0f }, lerpTime);
        vertPos[3] = Lerp(vertPos[3], { PHOTO_EXPANSION_POS_X + PHOTO_EXPANSION_SIZE_X,PHOTO_EXPANSION_POS_Y + PHOTO_EXPANSION_SIZE_Y,1.0f }, lerpTime);
    }

    //配列に数値を合わせて描画
    photoOrigShader_->DrawOnlyPixel(vertPos, photoTbl_[name].handle);
}

void InventoryScene::InitInventory(void)
{
    //インベントリのデータ読み込み
    tmxObj_.LoadTmx("resource/tmx/inventory.tmx", false);
    for (auto& inventory : inventoryList_)
    {
        //値の初期化とサイズの調整
        inventory.pos_ = { 0.0f,0.0f,0.0f };
        MV1SetScale(lpModelMng.GetID(inventory.modelString_)[inventory.modelNum_], VScale(inventory.scale_.toVECTOR(), 0.1f));
    }

    for (auto& coll : tmxObj_.GetInventoryCollList())
    {
        for (auto& itemTable : itemTable_)
        {
            if (coll.first == itemTable.first)
            {
                for (auto& inventory : inventoryList_)
                {
                    //値のセット
                    if (inventory.pos_ == Vector3{ 0.0f,0.0f,0.0f })
                    {
                        //値とインベントリの番地のセット
                        inventory.pos_ = { coll.second.first.x + OFFSET,lpSceneMng.GetScreenSize().y - coll.second.first.y - OFFSET,0 };
                        inventory.inventoryNum_ = itemTable.first;
                        break;
                    }
                }
            }
        }
    }
}

void InventoryScene::UpdateInventory(void)
{
    for (auto& photo : photoTbl_)
    {
        //拡大している場合は他の処理をしない
        if (photo.second.expansionFlag)
        {
            //決定を押した場合は拡大をやめる
            if (controller_->CheckInputKey(KeyID::Decide))
            {
                photo.second.stepLerpTime = PHOTO_LERP_TIME;
                photo.second.expansionFlag = false;
            }
            photo.second.stepLerpTime -= lpSceneMng.GetDeltaTime();
            return;
        }

    }

    hitItemID_ = ItemID::Non;
    Vector2Int mPos;
    GetMousePoint(&mPos.x, &mPos.y);

    for (auto& coll : tmxObj_.GetInventoryCollList())
    {
        for (auto& inventory : inventoryList_)
        {
            if (coll.first != inventory.inventoryNum_)
            {
                continue;
            }
            for (auto& itemTable : itemTable_)
            {
                if (itemTable.first != inventory.inventoryNum_)
                {
                    continue;
                }
                //マウスの当たった所の色を変える
                //どのIDか格納
                if (CheckCollMouse()(mPos, coll.second))
                {
                    hitItemID_ = inventory.itemID_;
                    itemTable.second = { 255,255,255 };
                    if (inventory.usedItemID_ != ItemID::Non)
                    {
                        itemTable.second = { 75,75,75 };
                    }
                }
                else
                {
                    itemTable.second = { 125,125,125 };
                    if (inventory.usedItemID_ != ItemID::Non)
                    {
                        itemTable.second = { 75,75,75 };
                    }
                }

                if (inventory.inventoryNum_ == lpSocketServer.GetinventorySelect())
                {
                    hitItemID_ = inventory.itemID_;
                    itemTable.second = { 255,255,255 };
                }
            }

        }
    }

    //写真とマウスの当たり判定
    CollMousePosAndPhoto();

    for (auto& inventory : inventoryList_)
    {
        //マウスが当たっている間　座標、大きさ、回転　を変える
        if (hitItemID_ == inventory.itemID_)
        {
            MV1SetPosition(lpModelMng.GetID(inventory.modelString_)[inventory.modelNum_], VGet(950,
                650, 0));
            Vector3 dd = inventory.scale_ * 1;
            //選択して際の回転を設定して正面に向かせる
            switch (inventory.itemID_)
            {
            case ItemID::Entrance_Key:
            case ItemID::Kitchen_Key:
            case ItemID::MusicRoom_Key:
            case ItemID::FoodBank_Key:
            case ItemID::ChildRoom_Key:
            case ItemID::Celler_Key:
                //鍵だった場合の回転をセットする
                MV1SetRotationXYZ(lpModelMng.GetID(inventory.modelString_)[inventory.modelNum_], Vector3(0, 0, 0).toVECTOR());
                break;
            case ItemID::Phot:
               
                //写真だったら場合、大きさを拡大しての回転をセットする
                dd = inventory.scale_ * 1.5f;
                MV1SetRotationXYZ(lpModelMng.GetID(inventory.modelString_)[inventory.modelNum_], Vector3(0, Deg2RadF(180), 0).toVECTOR());
                break;
            case ItemID::Mapitem:
                dd = inventory.scale_ * 1.8f;
                MV1SetRotationXYZ(lpModelMng.GetID(inventory.modelString_)[inventory.modelNum_], Vector3(Deg2RadF(90), 0, Deg2RadF(180)).toVECTOR());
                break;
            default:
                //上記以外のアイテムだったら-90度をいれて置く
                MV1SetRotationXYZ(lpModelMng.GetID(inventory.modelString_)[inventory.modelNum_], Vector3(Deg2RadF(-90), 0, 0).toVECTOR());
                break;
            }

            MV1SetScale(lpModelMng.GetID(inventory.modelString_)[inventory.modelNum_], VScale(dd.toVECTOR(), 1));

        }
        else
        {
            //座標、大きさをもとに戻す
            MV1SetPosition(lpModelMng.GetID(inventory.modelString_)[inventory.modelNum_], inventory.pos_.toVECTOR());
            MV1SetScale(lpModelMng.GetID(inventory.modelString_)[inventory.modelNum_], VScale(inventory.scale_.toVECTOR(), 0.1f));
        }
    }
}

void InventoryScene::DrawInventory(void)
{
    int color = FRAME_COLOR;
    for (auto& coll : tmxObj_.GetInventoryCollList())
    {
        for (auto& inventory : inventoryTable_)
        {
            if (coll.first == inventory.first)
            {
                //ストリングに何も入ってなかったら普通の四角を書く
                if (inventory.second == "")
                {
                    DrawBox(static_cast<int>(coll.second.first.x),
                        static_cast<int>(coll.second.first.y),
                        static_cast<int>(coll.second.first.x) + static_cast<int>(coll.second.second.x),
                        static_cast<int>(coll.second.first.y) + static_cast<int>(coll.second.second.y),
                        GetColor(color, color, color), false);
                    color += 1;
                }
                else
                {
                    //入っていたらそれを描画する
                    DrawExtendGraph(static_cast<int>(coll.second.first.x),
                        static_cast<int>(coll.second.first.y),
                        static_cast<int>(coll.second.first.x) + static_cast<int>(coll.second.second.x),
                        static_cast<int>(coll.second.first.y) + static_cast<int>(coll.second.second.y), lpImageMng.GetID(inventory.second)[0], true);
                    break;
                }
            }

        }
        //アイテムの枠の描画
        for (auto& itemTable : itemTable_)
        {
            if (coll.first == itemTable.first)
            {
                DrawBox(static_cast<int>(coll.second.first.x),
                    static_cast<int>(coll.second.first.y),
                    static_cast<int>(coll.second.first.x) + static_cast<int>(coll.second.second.x),
                    static_cast<int>(coll.second.first.y) + static_cast<int>(coll.second.second.y),
                    GetColor(static_cast<int>(itemTable.second.x), static_cast<int>(itemTable.second.y), static_cast<int>(itemTable.second.z)), false);
            }
        }
        /*if (coll.first == "target")
        {
            DrawFormatStringToHandle(static_cast<int>(coll.second.first.x + OFFSET), static_cast<int>(coll.second.first.y + OFFSET), 0x000000, font_, "玄関の鍵を見つけて脱出しよう");
            DrawFormatString(static_cast<int>(coll.second.first.x + OFFSET), static_cast<int>(coll.second.first.y + OFFSET * 2), 0x000000, "探してる物");
            DrawGraph(static_cast<int>(coll.second.first.x + OFFSET * 5), static_cast<int>(coll.second.first.y + OFFSET * 1.5), lpImageMng.GetID("resource/image/marker/marker.png")[0], true);
        }*/
        //写真描画
        DrawPhoto(coll.first, { coll.second.first }, { coll.second.second });
    }
    //モデル描画
    for (auto& inventory : inventoryList_)
    {
        origShader_->Draw(lpModelMng.GetID(inventory.modelString_)[inventory.modelNum_]);
        //MV1DrawModel(lpModelMng.GetID(inventory.modelString_)[inventory.modelNum_]);
    }
    //文字の描画（今はストリング最終的に画像）
    for (auto& coll : tmxObj_.GetInventoryCollList())
    {
        if (coll.first == "explanation")
        {
            for (auto& explanation : explanationTable_)
            {
                //ID確認
                if (hitItemID_ == explanation.first)
                {
                    //入っていたらそれを描画する
                    DrawExtendGraph(static_cast<int>(coll.second.first.x),
                        static_cast<int>(coll.second.first.y),
                        static_cast<int>(coll.second.first.x) + static_cast<int>(coll.second.second.x),
                        static_cast<int>(coll.second.first.y) + static_cast<int>(coll.second.second.y), lpImageMng.GetID(explanation.second)[0], true);
                }
            }
        }
    }
    for (auto& coll : tmxObj_.GetInventoryCollList())
    {
        for (auto& option : optionTable_)
        {
            if (coll.first == option.first)
            {
                DrawFormatStringToHandle(static_cast<int>(coll.second.first.x + OFFSET), static_cast<int>(coll.second.first.y + OFFSET / 2), 0xffffff, font_, "%s", option.second.c_str());
                if (isDrawTips_)
                {
                    if(coll.first == "invtyTips")
                    DrawBox(static_cast<int>(coll.second.first.x),
                        static_cast<int>(coll.second.first.y),
                        static_cast<int>(coll.second.first.x) + static_cast<int>(coll.second.second.x),
                        static_cast<int>(coll.second.first.y) + static_cast<int>(coll.second.second.y),
                        GetColor(144, 144, 144), false);
                }
            }
        }
    }
}

void InventoryScene::InitTable(void)
{
    inventoryTable_ = {
            {"belongings",	"resource/image/inventory/Inventory.png"},
            //{"target",	"resource/image/inventory/memo.png"},
            {"explanation",""}
    };
    Vector3 Zero = { 125,125,125 };
    itemTable_ = {
        {"1",Zero},
        {"2",Zero},
        {"3",Zero},
        {"4",Zero},
        {"5",Zero},
        {"6",Zero},
        {"7",Zero},
        {"8",Zero},
        {"9",Zero},
        {"10",Zero},
        {"11",Zero},
        {"12",Zero},
        {"13",Zero},
        {"14",Zero},
        {"15",Zero},
        {"16",Zero},
        {"17",Zero},
        {"18",Zero},
        {"19",Zero},
        {"20",Zero}
    };

    explanationTable_ = {
        {ItemID::Entrance_Key,"Entrancekeydescribe"},   //玄関の鍵の説明画像
        {ItemID::Kitchen_Key,"kitchenkeydescribe"},     //キッチンの鍵の説明画像
        {ItemID::MusicRoom_Key,"musicroomkeydescribe"}, //音楽室の鍵の説明画像
        {ItemID::FoodBank_Key,"Pantrykeydescribe"},     //食糧庫の鍵の説明画像
        {ItemID::ChildRoom_Key,"childRoomkeydescribe"}, //子供部屋の説明画像
        {ItemID::Celler_Key,"basementkeydescribe"},     //地下室の鍵の説明画像

        {ItemID::Enemy_Memo,"enemymemo"},               //敵の弱点の説明画像

        {ItemID::Story_Book_A,"storymemo_a"},           //ストーリーメモaの説明画像
        {ItemID::Story_Memo_B,"storymemo_b"},           //ストーリーメモbの説明画像
        {ItemID::Story_Book_C,"storymemo_c"},           //ストーリーメモc兼壁の説明画像1

        {ItemID::Wall_Memo,"hint_d"},                   //壁の説明画像2
        {ItemID::Picture_Memo,"hint_q"},                //応接間の絵画の説明画像
        {ItemID::CookWare_Memo,"hint_f"},               //調理器具の説明画像
        {ItemID::Score_Memo,"hint_g"},                  //楽譜の説明画像
        {ItemID::Living_Memo,"hint_h"},                 //ゲームソフトの説明画像
        {ItemID::Clock_Book,"hint_j"},                  //猫の時計と絵画の説明画像
        {ItemID::Celler_Memo,"hint_t"},                 //警告版の説明画像

        {ItemID::Phot,"Photdescide"},                   //写真の説明画像
        {ItemID::Mapitem,"mapItemdescide"}              //マップアイテムの説明画像


    };
    if (controller_->GetControllerType() == ControllerType::Key)
    {
        optionTable_ = {
                {"tab",	"tabでゲームに戻る"},
                {"mouse","カーソルを合わせてアイテムの確認"},
                {"invtyTips",""}
        };
    }
    else
    {
        optionTable_ = {
                {"tab",	"Yでゲームに戻る"},
                {"mouse","カーソルを合わせてアイテムの確認"},
                {"invtyTips",""}
        };
    }
}
