#pragma once
#include "../Obj.h"
#include"../../common/Geometry.h"
#include"../../common/Transform.h"
#include"../../common/Capsule.h"

class Stamina;
class Controller;
class MinCamera;
struct GimmickObj;
class Gimmick;
struct ItemObj;
struct InventoryObj;
class Item;
enum class ItemID;
enum class ControllerType;
class OriginalShader;

enum class State
{
    Walk,   //歩く
    Fall,   //落下
    Max
};

class Player :
    public Obj
{
public:
    Player(ControllerType type, const std::vector<std::shared_ptr<Gimmick>>& gList, const std::vector<std::shared_ptr<Item>>& iList);
    ~Player();

    //初期化
    void Init(void) override;
    //破棄
    void Release(void) override;
    //更新
    void Update(void) override;
    void Update(Vector3 cameraAngle,Vector3 targetPos);
    //描画
    void Draw(void) override;
    void Draw(int screenID);
    //位置を取得
    Vector3 GetPosition(void);
    bool GetminCameraFlag();
    bool GetIsGameClear();

    //ダッシュをしているか返す
    bool GetDashFlag(void);

    //カメラのレンダリングターゲット設定
    void SetMinCameraRenderTarget(int screenID);

    //フラッシュの当たり判定
    bool CheckHitFlash(Vector3 targetPos, Vector3 cameraAngle);

    //カメラの写真情報を返す
    std::vector<int>& GetMinCameraPhotoVector(void);

    //当たり判定用の変数を返す
    std::vector<Collider>& GetColliders(void);
    std::vector<InventoryObj>& GetInventoryList(void);
    Capsule GetCapsule(void);
    ItemID GetItemID(void);

    bool GetLowSensFlg(void);

private:
    //移動処理
    void MovePosition(Vector3 cameraAngle);
    //斜め移動の距離制限処理
    bool CheckDiagonalMove(void);

    bool HitLineX(float movePower, int& collision);
    bool HitLineZ(float movePower, int& collision);

    bool HitLineXGimmick(float movePower);
    bool HitLineZGimmick(float movePower);

    void SlideLinePosition(void);

    void HitCapsuleAndGoal(void);

    //インベントリへの追加処理
    void AddInventory(Vector3 targetPos);

    //担当.三ヶ島滉季(開始)---------------------------------------------------------------------------
    //インタラクト処理(ギミック)
    void InteractGimmick(Vector3 targetPos);
    
    //ギミックのイベント処理
    void GimmickEvent(Vector3 targetPos);

    //引き出しとアイテムをの動きをつなげる
    void ConnectingDrawerToItem(void);

    //ダイヤルカウント更新処理
    void DialCountUpdate(const std::shared_ptr<Gimmick> &gimmick,int modelNum,char numchar);

    //インベントリの中身を確認
    bool CheckInventory(const std::shared_ptr<Gimmick>& gimmick);
    //担当.三ヶ島滉季(終了)---------------------------------------------------------------------------

    //サウンド処理
    void Sound(void);

    //位置
    Vector3 pos_;       //現在
    Vector3 oldpos_;    //1フレーム前

    //回転
    Quaternion quaRot_;

    VECTOR PlayerRot_;
    //情報
    std::unique_ptr<Controller> controller_; //コントローラ   
    std::unique_ptr<Stamina> stamina_;       //スタミナ

    bool pickUpFlag_;
    bool catchFlg_;
    bool lowSensFlg_;
    //当たり判定用
    std::vector<Collider> colliders_;   //マップ関係
    Capsule capsule_;  //プレイヤーのカプセル
    std::unique_ptr<MinCamera> minCamera_;  //プレイヤーのカプセル

    //状態
    State state_;
    //重力
    float fallGravity_;

    //当たり判定用ハンドル
    int collisionHandle_;

    //リスト
    std::vector<std::shared_ptr<Gimmick>> gimmickList_;
    std::vector<std::shared_ptr<Item>> itemList_;

    std::vector<InventoryObj> inventoryList_;
    //ダイヤル番号
    int dial_[4];
    //ダイヤル番号(文字列)
    std::string dialNumStr_="0000";
    bool dashFlag_;

    bool isGameClear_;
    bool isHitGoal_;

    //プレイヤーが取得した鍵情報
    ItemID itemID_;
};
