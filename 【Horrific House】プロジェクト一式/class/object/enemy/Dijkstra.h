#pragma once
#include <list>
#include<memory>
#include"Node.h"
#include "../../common/Geometry.h"

enum  class ItemID;

class Dijkstra
{
public:
	Dijkstra();
	~Dijkstra();

    //初期化
    void Init(void);

    //更新
    Vector3 Update(bool huntFlag,Vector3 playerPos, ItemID itemID,bool alphaFlag);

    //デバッグ描画
    void Draw(void);

    //全ノードの削除
    void Clear(void);

    //追跡時にプレイヤーの位置にノードを追加
    void AddHuntNode(Vector3 playerPos, Vector3 enemyPos);
    //追跡時にプレイヤーの位置にノードを追加
    void ConnectNearNode(Vector3 enemyPos);

private:

    //ノードの追加
    void AddNode(Vector3 initPos);

    //ノードを接続する
    void ConnectNode(SharedNode n1, SharedNode n2);

    //ファイル読み込み
    bool ImportNodeData(std::string fileName);

    //ノードの削除
    void RemoveNode(SharedNode node);

    //追跡時に生成されたノードの削除処理
    void RemoveHuntNode(void);

    //経路探索処理
    void SearchRoot(void);

    //スタート地点とゴール地点をセット
    void SetStartAndGoal(SharedNode startNode, SharedNode goalNode);

    //スタート地点をセット
    void SetStart(SharedNode node);

    //ゴール地点をセット
    void SetGoal(SharedNode node);

    //ゴールに辿り着いた時の処理
    void SetNewGoalWhenArriveGoal(const bool huntFlag,const Vector3 playerPos);

    //距離を計算
    float CalcDistanceCost(SharedNode n1, SharedNode n2);

    //ノードリセット
    void ResetNode(void);

    //ノードがリストの何番目か
    int SearchIndex(SharedNode node);

    //インデックス指定でリストからノードを取得
    SharedNode GetNodeFromIndex(int idx);

    //アイテムを拾った場合にプレイヤーの近くのノードを目的地とする処理
    void SetNearPlayerNode(ItemID itemID);

    //敵関係
    Vector3 enemyPos_;       //位置
    SharedNode enemyNode_;   //現在いるノード

    //ノード関係
    SharedNode startNode_;       //スタートのノード
    SharedNode goalNode_;        //ゴールのノード
    SharedNode retrurnNode_;     //追跡が終わった後、敵のいる位置のノード
    std::list<SharedNode> node_; //ノードリスト

    int nodeNum_;                //現在のノード数
    int basicNodeNum_;           //普段のノード数(読み込んだノードの数)

    //壁透過時の移動フラグ
    bool wallAlphaFlag_;
};
