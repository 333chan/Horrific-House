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

    //������
    void Init(void);

    //�X�V
    Vector3 Update(bool huntFlag,Vector3 playerPos, ItemID itemID,bool alphaFlag);

    //�f�o�b�O�`��
    void Draw(void);

    //�S�m�[�h�̍폜
    void Clear(void);

    //�ǐՎ��Ƀv���C���[�̈ʒu�Ƀm�[�h��ǉ�
    void AddHuntNode(Vector3 playerPos, Vector3 enemyPos);
    //�ǐՎ��Ƀv���C���[�̈ʒu�Ƀm�[�h��ǉ�
    void ConnectNearNode(Vector3 enemyPos);

private:

    //�m�[�h�̒ǉ�
    void AddNode(Vector3 initPos);

    //�m�[�h��ڑ�����
    void ConnectNode(SharedNode n1, SharedNode n2);

    //�t�@�C���ǂݍ���
    bool ImportNodeData(std::string fileName);

    //�m�[�h�̍폜
    void RemoveNode(SharedNode node);

    //�ǐՎ��ɐ������ꂽ�m�[�h�̍폜����
    void RemoveHuntNode(void);

    //�o�H�T������
    void SearchRoot(void);

    //�X�^�[�g�n�_�ƃS�[���n�_���Z�b�g
    void SetStartAndGoal(SharedNode startNode, SharedNode goalNode);

    //�X�^�[�g�n�_���Z�b�g
    void SetStart(SharedNode node);

    //�S�[���n�_���Z�b�g
    void SetGoal(SharedNode node);

    //�S�[���ɒH�蒅�������̏���
    void SetNewGoalWhenArriveGoal(const bool huntFlag,const Vector3 playerPos);

    //�������v�Z
    float CalcDistanceCost(SharedNode n1, SharedNode n2);

    //�m�[�h���Z�b�g
    void ResetNode(void);

    //�m�[�h�����X�g�̉��Ԗڂ�
    int SearchIndex(SharedNode node);

    //�C���f�b�N�X�w��Ń��X�g����m�[�h���擾
    SharedNode GetNodeFromIndex(int idx);

    //�A�C�e�����E�����ꍇ�Ƀv���C���[�̋߂��̃m�[�h��ړI�n�Ƃ��鏈��
    void SetNearPlayerNode(ItemID itemID);

    //�G�֌W
    Vector3 enemyPos_;       //�ʒu
    SharedNode enemyNode_;   //���݂���m�[�h

    //�m�[�h�֌W
    SharedNode startNode_;       //�X�^�[�g�̃m�[�h
    SharedNode goalNode_;        //�S�[���̃m�[�h
    SharedNode retrurnNode_;     //�ǐՂ��I�������A�G�̂���ʒu�̃m�[�h
    std::list<SharedNode> node_; //�m�[�h���X�g

    int nodeNum_;                //���݂̃m�[�h��
    int basicNodeNum_;           //���i�̃m�[�h��(�ǂݍ��񂾃m�[�h�̐�)

    //�Ǔ��ߎ��̈ړ��t���O
    bool wallAlphaFlag_;
};
