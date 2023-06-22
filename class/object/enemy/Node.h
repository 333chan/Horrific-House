#pragma once
#include <list>
#include<memory>
#include "../../common/Geometry.h"

struct Node;

using SharedNode = std::shared_ptr<Node>;

struct NodeConnection
{
    SharedNode node;   //移動先ノード
    float cost;   //移動距離

    NodeConnection(SharedNode n, float c)
    {
        node = n;
        cost = c;
    }
};

struct NodePair
{
    SharedNode n1;
    SharedNode n2;
};

struct Node
{
    Node(Vector3 initPos);
    ~Node();

    //ノードのリセット
    void Reset(void);

    //特定ノードへの接続を削除
    void RemoveConnect(SharedNode node);

    //位置
    Vector3 pos;

    //接続ノード
    std::list<NodeConnection> connectNode;

    float cost = -1;       //探索コスト
    SharedNode toGoal;     //ゴールへの最短ルートに繋がるノード
};

