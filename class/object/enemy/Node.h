#pragma once
#include <list>
#include<memory>
#include "../../common/Geometry.h"

struct Node;

using SharedNode = std::shared_ptr<Node>;

struct NodeConnection
{
    SharedNode node;   //�ړ���m�[�h
    float cost;   //�ړ�����

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

    //�m�[�h�̃��Z�b�g
    void Reset(void);

    //����m�[�h�ւ̐ڑ����폜
    void RemoveConnect(SharedNode node);

    //�ʒu
    Vector3 pos;

    //�ڑ��m�[�h
    std::list<NodeConnection> connectNode;

    float cost = -1;       //�T���R�X�g
    SharedNode toGoal;     //�S�[���ւ̍ŒZ���[�g�Ɍq����m�[�h
};

