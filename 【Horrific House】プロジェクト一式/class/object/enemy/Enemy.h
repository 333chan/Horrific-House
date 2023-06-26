#pragma once
#include<list>
#include "../Obj.h"
#include"../../common/Transform.h"
#include "../../common/Geometry.h"
#include"../../common/Capsule.h"

class AnimationController;
class Dijkstra;
class Gimmick;
class OriginalShader;
enum class ItemID;
struct NodeConnection;

//アニメーション
enum EnemyAnimation
{
    Idle,               //立ち
    Run,                //走る
    Damage,             //ひるみ
    FemaleDynamicPose,  //空飛び
    Walk,               //歩き
    Scream,             //叫ぶ
    Max
};

class Enemy :
    public Obj
{
public:
    Enemy(const std::vector<std::shared_ptr<Gimmick>>& gList);
    Enemy(Vector3 pos);
    ~Enemy();

    //初期化
    void Init(void) override;
    void Init(Vector3 pos);//pos:敵の位置
    //破棄
    void Release(void) override;
    //更新
    void Update(void) override;
    void Update(Capsule capsule,const Vector3 playerPos,bool hitFlash,ItemID itemID, const std::vector<std::shared_ptr<Gimmick>>& gimmick);
    void PKMotion(Vector3 TargetPos);//プレイヤーを殺すMotion
    //描画
    void Draw(void) override;

    //位置を返す
    Vector3 GetPosition(void);
    //向いている方向を取得
    Vector3 GetDirectionVec(void);
    Vector3 GetTargetLine(void);
    //プレイヤーとの衝突フラグを返す
    bool GetHitPlayerFlag(void);

    //追跡フラグを返す
    bool GetHuntFlag(void);

private:
    //プレイヤーを探索する処理
    void SearchPlayer(const Vector3 playerPos);

    //プレイヤー探索(ture:発見/false:発見していない)
    bool Search(const Vector3 playerPos);

    //プレイヤーを追跡する処理
    void HuntPlayer(void);

    //ダイクストラ処理
    void DijkstraProcess(Vector3 playerPos,ItemID itemID, bool alphaFlag);

    //衝突処理
    bool CheckHitPlayer(void);

    //フラッシュを受けたときの処理
    void DamageFlash(bool hitFlash);

    //フラッシュ状態から回復する処理
    void RecoverFlash(void);

    //スタン時間回復
    void RecoverStanTime(void);

    //スタン時間短縮
    void ReduceStanTime(void);

    //ターゲットの方向を向く
    void Lock_On_Target(Vector3 targetPos);
    //モデル制御
    Transform transform_;

    //アニメーションコントローラ
    std::unique_ptr<AnimationController> animController_;

    //追跡関係
    bool huntFlag_;         //プレイヤー発見フラグ
    double stepHuntTime_;   //追跡時間

    //目的地
    Vector3 goalPos_;
    //向いている方向のベクトル
    Vector3 searchDir_;
    //回転
    Quaternion targetRot_;  //目的の方向
    double stepRotTime_;    //回転時間

    //スタン関係
    bool stanFlag_;         //スタンフラグ
    float nowStanTime_;      //敵のスタン時間(スタンするほど短くなる)
    double stepStanTime_;   //スタン時間

    //ダイクストラ情報
    std::unique_ptr<Dijkstra> dijkstra_;
    //ギミック情報
    std::vector<std::shared_ptr<Gimmick>> gimmickList_;

    //プレイヤーのカプセル情報
    Capsule playerCapsule_;

    //プレイヤー衝突フラグ
    bool hitPlayerFlag_;

    //壁透過フラグ(敵側で用意しないと常に追跡モードとなる)
    bool wallAlphaFlag_;

    //目線の先
    Vector3 targetPos_;
};
