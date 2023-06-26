#pragma once

#include "../Obj.h"
#include "../../common/Geometry.h"

enum class ItemID
{
    Non,
    Entrance_Key,   //玄関の鍵
    Kitchen_Key,    //キッチンの鍵
    MusicRoom_Key,  //音楽室の鍵
    FoodBank_Key,   //食糧庫の鍵
    ChildRoom_Key,  //子供部屋の鍵
    Celler_Key,     //地下室の鍵
    Phot,           //写真
    Mapitem,        //マップアイテム
    Story_Memo_B,   //ストーリーBに関するメモ
    Enemy_Memo,     //敵に関するメモ
    Wall_Memo,      //壁の写真に関するメモ
    Picture_Memo,   //絵画に関するメモ
    CookWare_Memo,  //調理器具に関するメモ
    Score_Memo,     //楽譜に関するメモ
    Living_Memo,    //リビングに関するメモ
    Celler_Memo,    //地下室に関するメモ
    Dummy_Memo,    //地下室に関するメモ
    Story_Book_A,   //ストーリーAに関するメモ
    Story_Book_C,   //ストーリーCに関するメモ
    Clock_Book,     //時計に関するメモ
    Dummy_Book,     //時計に関するメモ
    LeftNumber,     //ダイヤルキーの左側のヒント
    RightNumber,    //右側のヒント
    Max
};
//アイテム移動用
enum class ItemMove
{
    Open,       //開いている時
    Close,      //閉まっている時
    Max
};

//アイテムのアニメーション状態
enum class ItemAnim
{
    Playing,    //再生(移動)
    Stop,       //停止
    Max
};

//アイテムオブジェクト情報
struct ItemObj
{
    std::string objName_;   //オブジェクト名
    Vector3 homePos_;       //座標
    Vector3 movedPos_;      //座標
    Vector3 angle_;	        //回転
    Vector3 scale_;	        //拡大値
    ItemObj(std::string objName, const Vector3& bPos, const Vector3& aPos, const Vector3& angle, const Vector3& scale) :
        objName_(objName),
        homePos_(bPos),
        movedPos_(aPos),
        angle_(angle),
        scale_(scale) {};
};


struct InventoryObj
{
    std::string modelString_;       //モデルハンドル
    int modelNum_;                  //複製数
    ItemID itemID_;                 //アイテムID
    Vector3 pos_;                   //座標
    Vector3 scale_;                 //拡大値
    std::string inventoryNum_;      //インベントリ数
    ItemID usedItemID_;             //アイテムID
};

class Item
    :public Obj
{
public:
    Item();
    ~Item();
    virtual void Draw(bool cameraflag) {};
   

    
    /// <summary>
    /// モデルハンドルを返す
    /// </summary>
    /// <param name="void"></param>
    /// <returns></returns>
    virtual std::string GetModelString(void) { return modelString_; }
    
    /// <summary>
    /// オブジェクト名を返す
    /// </summary>
    /// <param name="name_"></param>
    /// <returns></returns>
    virtual std::string GetName(void) { return name_; }

    //アイテムIDを返す
    virtual ItemID GetItemID(void) = 0;

    //モデルの複製番号を返す
    virtual int GetModelNumber(void) { return modelNum_; }

    //取得フラグをセット
    virtual void SetPickUpFlg(bool flg) { pickupFlg_ = flg; }

    //取得フラグを返す
    virtual bool GetPickUpFlg(void) { return pickupFlg_; }

    //当たり判定フラグを返す
    virtual void SetHitFlg(bool flg) { hitFlg_ = flg; }

    //移動判定用フラグを返す
    virtual void SetMoveFlg(bool flg) { moveFlg_=flg; }

    //ロック状態フラグを返す
    virtual bool GetLockFlg(void) { return lockFlg_; }


private:
protected:
    //モデルのファイルパス
    std::string modelString_;

    //オブジェクト名
    std::string name_;

    //モデル複製数
    int modelNum_;

    //ライティング
    int vsLight_;
    int psLight_;

    //座標
    Vector3 pos_;

    //初期座標
    Vector3 homePos_;

    //移動後座標
    Vector3 movedPos_;

    //角度
    Vector3 angle_;

    //拡大値
    Vector3 scale_;

    //取得フラグ
    bool pickupFlg_;

    //移動フラグ
    bool moveFlg_;

    //ロックフラグ
    bool lockFlg_;

    //当たり判定用フラグ
    bool hitFlg_;

    //アイテムID
    ItemID itemId_;
};

