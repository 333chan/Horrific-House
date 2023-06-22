#include<fstream>
#include<iostream>
#include<string>
#include<time.h>
#include "Dijkstra.h"
#include"../../tmx/TmxObj.h"
#include"../Item/Item.h"
#include "../../../_debug/_DebugConOut.h"

constexpr float HUNT_COEFFICIENT = 2.75f;    //追跡時の移動係数
constexpr float MOVE_SPEED = 25.0f;	        //移動スピード

//キーの近くのノード番号
constexpr int CHILD_KEY_PLACE_NODE_NUM = 76;            //子供部屋
constexpr int ENTRANCE_KEY_PLACE_START_NODE_NUM = 20;   //エントランスのスタートノード
constexpr int ENTRANCE_KEY_PLACE_GOAL_NODE_NUM = 84;    //エントランスのゴールノード
constexpr int KITCHEN_KEY_PLACE_NODE_NUM = 6;           //キッチン
constexpr int BANK_KEY_PLACE_NODE_NUM = 26;             //食糧庫
constexpr int MUSIC_KEY_PLACE_NODE_NUM = 7;             //音楽室
constexpr int PHOTO_PLACE_NODE_NUM = 60;                //写真

//壁透過時のノード
constexpr int ALPHA_WALL_NEAR_START_NODE_NUM = 19;      //スタート
constexpr int ALPHA_WALL_NEAR_GOAL_NODE_NUM = 14;      //ゴール

//オフセット
constexpr float POS_OFFSET_Y = 300.0f;

Dijkstra::Dijkstra()
{
    Init();
}

Dijkstra::~Dijkstra()
{
    Clear();
}

void Dijkstra::Init(void)
{
    startNode_ = nullptr;
    goalNode_ = nullptr;
    retrurnNode_ = nullptr;
    nodeNum_ = 0;
    srand(static_cast<unsigned int>(time(nullptr)));

    ImportNodeData("./resource/data/enemy/node.tsx");

    enemyNode_ = startNode_;
    enemyPos_ = enemyNode_->pos;
    wallAlphaFlag_ = false;
    SearchRoot();
}

Vector3 Dijkstra::Update(bool huntFlag, Vector3 playerPos, ItemID itemID, bool alphaFlag)
{
    if (!alphaFlag && !wallAlphaFlag_)
    {
        wallAlphaFlag_ = true;
        enemyPos_ = GetNodeFromIndex(ALPHA_WALL_NEAR_START_NODE_NUM)->pos;
        SetStartAndGoal(GetNodeFromIndex(ALPHA_WALL_NEAR_START_NODE_NUM), GetNodeFromIndex(ALPHA_WALL_NEAR_GOAL_NODE_NUM));
        enemyNode_ = startNode_;
    }

    //敵のノードがnullではない場合はゴールに向かうノードに移動する
    if (enemyNode_->toGoal != nullptr)
    {
        float moveCoefficient = huntFlag ? HUNT_COEFFICIENT : 1.0f;
        //ノードのベクトルに移動
        auto vecN = (enemyNode_->toGoal->pos - enemyNode_->pos).Normalized();
        enemyPos_ += vecN * (MOVE_SPEED * moveCoefficient);

        //移動スピードより小さい場合は次のノードの位置へ移動
        if ((enemyNode_->toGoal->pos - enemyPos_).Magnitude() < MOVE_SPEED * moveCoefficient)
        {
            //次のノード
            enemyPos_ = enemyNode_->toGoal->pos;
            enemyNode_ = enemyNode_->toGoal;
        }
        if (enemyNode_->toGoal == nullptr)
        {
            SetNewGoalWhenArriveGoal(huntFlag, playerPos);
        }
    }
    else
    {
        //保険としてnullptrだった場合はゴールの配置をし直す
        SetStartAndGoal(goalNode_, GetNodeFromIndex(rand() % basicNodeNum_));
    }

    SetNearPlayerNode(itemID);

    return enemyPos_;
}

void Dijkstra::Draw(void)
{
#ifdef _DEBUG
    //デバッグ表示
    VECTOR sPos{};
    VECTOR ePos{};
    int num = 0;
    //点描画
    for (auto& node : node_)
    {
        auto pos = node->pos.toVECTOR();
        pos.y += POS_OFFSET_Y;

        DrawSphere3D(pos, 50.0f, 20, 0xff0000, 0xff0000, true);
        VECTOR screenPos = ConvWorldPosToScreenPos(pos);
        if (!CheckCameraViewClip(pos))
        {
            DrawFormatStringF(screenPos.x, screenPos.y, 0xffffff, "%d", num);
        }

        //全ての経路
        for (auto& c : node->connectNode)
        {
            sPos = pos;
            ePos = c.node->pos.toVECTOR();
            ePos.y += POS_OFFSET_Y;

            DrawLine3D(sPos, ePos, 0xffdddd);
        }
        num++;
    }

    //スタートからゴールへの経路
    if (startNode_ && startNode_->cost != -1)
    {
        SharedNode curr = startNode_;
        while (curr->toGoal)
        {
            sPos = curr->pos.toVECTOR();
            ePos = curr->toGoal->pos.toVECTOR();
            sPos.y += POS_OFFSET_Y;
            ePos.y += POS_OFFSET_Y;
            DrawLine3D(sPos, ePos, 0xff0000);
            curr = curr->toGoal;
        }
    }
#endif // _DEBUG
}

void Dijkstra::Clear(void)
{
    node_.clear();
    startNode_ = nullptr;
    goalNode_ = nullptr;
    //startNode_ = nullptr;
    //goalNode_ = nullptr;
}

void Dijkstra::SetStartAndGoal(SharedNode startNode, SharedNode goalNode)
{
    SetStart(startNode);
    SetGoal(goalNode);
    SearchRoot();
}

void Dijkstra::AddHuntNode(Vector3 playerPos, Vector3 enemyPos)
{
    //敵の位置のノード
    AddNode(enemyPos);
    //プレイヤーの位置のノード
    AddNode(playerPos);

    auto enemyNode = GetNodeFromIndex(static_cast<int>(node_.size()) - 2);
    auto playerNode = GetNodeFromIndex(static_cast<int>(node_.size()) - 1);

    ConnectNode(playerNode, enemyNode);

    //初期値はとても大きくする
    float minDis = 99999.9f;
    SharedNode minNode = nullptr;

    //敵の位置のノードと近くのノードを繋げる
    for (int n = 0; n < basicNodeNum_; n++)
    {
        const auto node = GetNodeFromIndex(n);
        //自分自身の場合は返す(保険)
        if (node == enemyNode || node == playerNode)
        {
            continue;
        }

        if (abs(node->pos.y - enemyNode->pos.y) > 0.1f)
        {
            continue;
        }

        float dis = CalcDistanceCost(enemyNode, node);

        if (minDis > dis)
        {
            minDis = dis;
            minNode = node;
        }
    }

    //同じ高さにノードがない場合は一番近いノードに繋げる
    if (minNode == nullptr)
    {
        for (int n = 0; n < basicNodeNum_; n++)
        {
            const auto node = GetNodeFromIndex(n);
            if (node == enemyNode || node == playerNode)
            {
                continue;
            }

            float dis = CalcDistanceCost(enemyNode, node);;

            if (minDis > dis)
            {
                minDis = dis;
                minNode = node;
            }
        }
    }

    ConnectNode(enemyNode, minNode);
    SetGoal(playerNode);
    enemyNode_ = enemyNode;
    SearchRoot();
}

void Dijkstra::ConnectNearNode(Vector3 enemyPos)
{
    //大きい数字
    float minDis = 99999.9f;
    SharedNode minNode = nullptr;

    //ゴールノードと近くのノードを繋げる
    for (int n = 0; n < basicNodeNum_; n++)
    {
        auto node = GetNodeFromIndex(n);
        if (goalNode_ == node)
        {
            continue;
        }

        if (abs(node->pos.y - goalNode_->pos.y) > 1.0f)
        {
            continue;
        }

        float dis = CalcDistanceCost(goalNode_, node);

        if (minDis > dis)
        {
            minDis = dis;
            minNode = node;
        }
    }

    if (minNode == nullptr)
    {
        return;
    }

    retrurnNode_ = minNode;

    ConnectNode(goalNode_, minNode);
    SetGoal(minNode);
    SearchRoot();
}

bool Dijkstra::ImportNodeData(std::string fileName)
{
    std::string name;

    Clear();

    rapidxml::file<> file = fileName.c_str();
    rapidxml::xml_document<> document;
    document.parse<0>(file.data());
    rapidxml::xml_node<>* posNode = document.first_node("NodePos");

    //ノードの位置を読み込み
    int num = 1;
    name = posNode->first_attribute("name")->value();
    if (name == "NodePos")
    {
        for (auto node = posNode->first_node("Pos1"); node != nullptr; node = node->next_sibling())
        {
            name = node->first_attribute("name")->value();
            if (name == "Pos" + std::to_string(num))
            {
                Vector3 pos = Vector3(
                    static_cast<float>(atof(node->first_attribute("posX")->value())),
                    static_cast<float>(atof(node->first_attribute("posY")->value())),
                    static_cast<float>(atof(node->first_attribute("posZ")->value()))
                );
                AddNode(pos);
            }
            num++;
        }
    }

    nodeNum_ = num - 1;
    basicNodeNum_ = nodeNum_;

    //スタート位置を読み込み
    auto startPosNode = posNode->next_sibling();
    name = startPosNode->first_attribute("name")->value();
    if (name == "StartNode")
    {
        startNode_ = GetNodeFromIndex(atoi(startPosNode->first_attribute("index")->value()));
    }

    //ゴール位置を読み込み
    auto goalPosNode = startPosNode->next_sibling();
    name = goalPosNode->first_attribute("name")->value();
    if (name == "GoalNode")
    {
        goalNode_ = GetNodeFromIndex(atoi(goalPosNode->first_attribute("index")->value()));
    }

    //接続ノードを読み込み
    num = 1;
    auto connectNode = goalPosNode->next_sibling();
    name = connectNode->first_attribute("name")->value();
    if (name == "ConnectNode")
    {
        for (auto node = connectNode->first_node("Connect1"); node != nullptr; node = node->next_sibling())
        {
            name = node->first_attribute("name")->value();
            if (name == "Connect" + std::to_string(num))
            {
                ConnectNode(
                    GetNodeFromIndex(atoi(node->first_attribute("index1")->value())),
                    GetNodeFromIndex(atoi(node->first_attribute("index2")->value()))
                );
            }
            num++;
        }
    }

    return true;
}

void Dijkstra::AddNode(Vector3 initPos)
{
    for (auto& n : node_)
    {
        if (n->pos == initPos)
        {
            return;
        }
    }
    node_.emplace_back(std::make_shared<Node>(initPos));
    nodeNum_++;
}

void Dijkstra::RemoveNode(SharedNode node)
{
    for (auto& n : node->connectNode)
    {
        n.node->RemoveConnect(node);
    }

    node_.remove(node);
    //sharedをdeleteするには reset?
    node.reset();
}

void Dijkstra::RemoveHuntNode(void)
{
    if (retrurnNode_ != nullptr)
    {
        int removeNum = nodeNum_ - basicNodeNum_;
        for (int n = 0; n < removeNum; n++)
        {
            auto itr = node_.end();
            itr--;
            RemoveNode(*itr);
        }
        retrurnNode_ = nullptr;
        nodeNum_ = basicNodeNum_;
    }
}

void Dijkstra::ConnectNode(SharedNode n1, SharedNode n2)
{
    //登録済みの場合は返す
    for (auto& cn : n1->connectNode)
    {
        if (cn.node == n2)
        {
            return;
        }
    }

    //距離の計算
    auto cost = CalcDistanceCost(n1, n2);

    //接続
    n1->connectNode.emplace_back(NodeConnection(n2, cost));
    n2->connectNode.emplace_back(NodeConnection(n1, cost));
}

void Dijkstra::SearchRoot(void)
{
    //リセット
    ResetNode();

    //nullの場合は返す
    if (!startNode_ || !goalNode_)
    {
        return;
    }

    std::list<SharedNode> work1;
    std::list<SharedNode> work2;

    std::list<SharedNode>* nowLevel = &work1;
    std::list<SharedNode>* nextLevel = &work2;
    std::list<SharedNode>* swapLevel = nullptr;

    //ゴールから計算
    goalNode_->cost = 0;
    nowLevel->emplace_back(goalNode_);

    float nodeCost = 0.0f;

    while (nowLevel->size())
    {
        for (const auto& level : *nowLevel)
        {
            for (auto& c : level->connectNode)
            {
                nodeCost = level->cost + c.cost;

                if (c.node->cost == -1 || nodeCost < c.node->cost)
                {
                    //未探索ノードあるいは最短ルートを更新できる場合
                    //経路コストとゴールへ向かうためのノードをセット
                    c.node->cost = nodeCost;
                    c.node->toGoal = level;
                }
                else
                {
                    continue;
                }
                nextLevel->push_back(c.node);
            }
        }

        //リストを入れ替えて次の階層を探索する
        swapLevel = nowLevel;
        nowLevel = nextLevel;
        nextLevel = swapLevel;
        nextLevel->clear();
    }

}

void Dijkstra::SetStart(SharedNode node)
{
    startNode_ = node;
}

void Dijkstra::SetNewGoalWhenArriveGoal(const bool huntFlag, const Vector3 playerPos)
{
    //ゴールに辿り着いた場合
    if (!huntFlag)
    {
        //追跡フラグがない場合は普通に移動
        //追跡ノード削除
        RemoveHuntNode();
        //新しいゴールを乱数で決める
        SetStartAndGoal(goalNode_, GetNodeFromIndex(rand() % basicNodeNum_));
    }
    else
    {
        //追跡フラグがある場合はプレイヤーの位置にノードを追加して現在のノードと繋げる
        AddNode(playerPos);
        ConnectNode(enemyNode_, GetNodeFromIndex(static_cast<int>(node_.size()) - 1));
        enemyNode_ = goalNode_;
        SetGoal(GetNodeFromIndex(static_cast<int>(node_.size()) - 1));
        SearchRoot();
    }
    enemyPos_ = enemyNode_->pos;
}

void Dijkstra::SetGoal(SharedNode node)
{
    goalNode_ = node;
}

float Dijkstra::CalcDistanceCost(SharedNode n1, SharedNode n2)
{
    return (n1->pos - n2->pos).Magnitude();
}

void Dijkstra::ResetNode(void)
{
    for (auto& node : node_)
    {
        node->Reset();
    }
}

int Dijkstra::SearchIndex(SharedNode node)
{
    int ret = 0;
    for (auto& n : node_)
    {
        if (n == node)
        {
            return ret;
        }
        ret++;
    }
    return -1;
}

SharedNode Dijkstra::GetNodeFromIndex(int idx)
{
    int ret = 0;
    
    for (auto& itr : node_)
    {
        if (ret == idx)
        {
            return itr;
        }
        ret++;
    }
    return nullptr;
}

void Dijkstra::SetNearPlayerNode(ItemID itemID)
{
    //何も手に入れてない場合は返す
    if (itemID == ItemID::Non)
    {
        return;
    }

    enemyPos_ = enemyNode_->pos;

    switch (itemID)
    {
    case ItemID::Entrance_Key:
        enemyNode_ = GetNodeFromIndex(ENTRANCE_KEY_PLACE_START_NODE_NUM);
        enemyPos_ = enemyNode_->pos;
        SetStartAndGoal(GetNodeFromIndex(ENTRANCE_KEY_PLACE_START_NODE_NUM), GetNodeFromIndex(ENTRANCE_KEY_PLACE_GOAL_NODE_NUM));
        break;
    case ItemID::Kitchen_Key:
        SetStartAndGoal(enemyNode_, GetNodeFromIndex(KITCHEN_KEY_PLACE_NODE_NUM));
        break;
    case ItemID::MusicRoom_Key:
        SetStartAndGoal(enemyNode_, GetNodeFromIndex(MUSIC_KEY_PLACE_NODE_NUM));
        break;
    case ItemID::FoodBank_Key:
        SetStartAndGoal(enemyNode_, GetNodeFromIndex(BANK_KEY_PLACE_NODE_NUM));
        break;
    case ItemID::ChildRoom_Key:
        SetStartAndGoal(enemyNode_, GetNodeFromIndex(CHILD_KEY_PLACE_NODE_NUM));
        break;
    case ItemID::Phot:
        SetStartAndGoal(enemyNode_, GetNodeFromIndex(PHOTO_PLACE_NODE_NUM));
        break;
    default:
        break;
    }
}
