#pragma once
#include "../Obj.h"
#include "../../common/Geometry.h"
#include "../../common/manager/ModelMng.h"
#include "../../common/manager/ImageMng.h"

//ギミックアニメーション種類
enum class GIMMICK_ANIM {
    NONE,
    OPEN,   //開ける
    CLOSE,  //閉じる
    MAX,
};

enum class GIMMICK_TYPE {
    NONE,
    DOOR,       //扉
    FRAME,      //オブジェクトの枠
    DRAWER,     //引き出し
    DIAL,       //ダイヤル
    DL_DOOR,    //ダイヤルでロックされている扉
    TEXT,       //文字
    WALL,       //壁
    WATER,      //水
    JUKE_BOX,   //ジュークボックス
    MAX
};

//ギミックオブジェクト情報
struct GimmickObj
{
    std::string objName_;   //オブジェクト名
    Vector3 pos_;           //座標
    Vector3 angle_;	        //回転
    Vector3 scale_;	        //拡大値
    GimmickObj(std::string objName,const Vector3& pos, const Vector3& angle, const Vector3& scale) :
        objName_(objName),
        pos_(pos),
        angle_(angle),
        scale_(scale) {}
};

//アイテムの種類
enum class ItemID;

class OriginalShader;

class Gimmick :
    public Obj
{
public:
    Gimmick();
    ~Gimmick();
    
    /// <summary>
    /// 座標を返す
    /// </summary>
    /// <param name=""></param>
    /// <returns></returns>
    virtual Vector3 GetPos(void) { return pos_; }

    /// <summary>
    /// モデルの数を返す
    /// </summary>
    /// <param name="void"></param>
    /// <returns></returns>
    virtual int GetModelNum(void) = 0;

    /// <summary>
    /// モデルハンドルの文字列を返す
    /// </summary>
    /// <param name=""></param>
    /// <returns></returns>
    virtual std::string GetCollModelStr(void) = 0;

    /// <summary>
    /// オブジェクト名を返す
    /// </summary>
    /// <param name="void"></param>
    /// <returns></returns>
    virtual std::string GetName(void) { return name_; }

    /// <summary>
    /// ロック状態を返す
    /// </summary>
    /// <param name=""></param>
    /// <returns></returns>
    virtual bool GetIsLockFlg(void) { return isLock_; }

    /// <summary>
    /// ロック状態をセット
    /// </summary>
    /// <param name="lockFlg"></param>
    virtual void SetIsLockFlg(bool lockFlg) { isLock_ = lockFlg; }

    /// <summary>
    /// インタラクトフラグセット
    /// </summary>
    /// <param name="再生フラグ"></param>
    virtual void SetInteractFlg(bool interactFlg) { interactFlg_ = interactFlg; }
    virtual bool GetInteractFlg(void) { return interactFlg_; }
    virtual void SetHitFlg(bool hitFlg) { hitFlg_ = hitFlg; }

    /// <summary>
    /// 当たった時の法線を設定
    /// </summary>
    /// <param name="hitNormal"></param>
    virtual void SetHitNormal(VECTOR hitNormal) { hitNormal_ = hitNormal; }

    /// <summary>
    /// 扉を開けるために必要な鍵IDを返す
    /// </summary>
    /// <returns></returns>
    virtual ItemID GetItemID() { return itemID_; };

    /// <summary>
    /// ダイアル番号を返す
    /// </summary>
    /// <param name=""></param>
    /// <returns></returns>
    virtual int GetDialNumber(void) { return dialNumber_; }

    /// <summary>
    /// 解除番号を返す
    /// </summary>
    /// <param name=""></param>
    /// <returns></returns>
    virtual std::string GetUnlockNumber(void) { return unlockNum_;  }

    /// <summary>
    /// ギミックタイプを返す
    /// </summary>
    /// <param name=""></param>
    /// <returns></returns>
    virtual GIMMICK_TYPE GetType(void) { return type_; }

    /// <summary>
    /// アニメーション状態を返す
    /// </summary>
    /// <param name=""></param>
    /// <returns></returns>
    virtual GIMMICK_ANIM GetAnimState_(void) { return animState_; }
private:
protected:
    //オブジェクト名
    std::string name_;

     //座標
    Vector3 pos_;

   //拡大値
    Vector3 scale_;

    //回転
    Vector3 angle_;

    //モデル複製数
    int modelNum_;

    //ダイアルの数字
    int dialNumber_;

    //ロック状態
    bool isLock_;

    //インタラクトフラグ
    bool interactFlg_;

    // 音声再生フラグ
    bool soundFlg_;

    //当たり判定フラグ
    bool hitFlg_;

    //モデルのファイルパス
    std::string modelStr_;

    //ロック解除の番号
    std::string unlockNum_;

    //ロックされている扉を開けるために必要な鍵のID
    ItemID itemID_;

    //あたった時の法線
    VECTOR hitNormal_;

    //ギミックの種類
    GIMMICK_TYPE type_;

    //アニメーション状態
    GIMMICK_ANIM animState_;
};