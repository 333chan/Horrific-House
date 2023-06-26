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

//�A�j���[�V����
enum EnemyAnimation
{
    Idle,               //����
    Run,                //����
    Damage,             //�Ђ��
    FemaleDynamicPose,  //����
    Walk,               //����
    Scream,             //����
    Max
};

class Enemy :
    public Obj
{
public:
    Enemy(const std::vector<std::shared_ptr<Gimmick>>& gList);
    Enemy(Vector3 pos);
    ~Enemy();

    //������
    void Init(void) override;
    void Init(Vector3 pos);//pos:�G�̈ʒu
    //�j��
    void Release(void) override;
    //�X�V
    void Update(void) override;
    void Update(Capsule capsule,const Vector3 playerPos,bool hitFlash,ItemID itemID, const std::vector<std::shared_ptr<Gimmick>>& gimmick);
    void PKMotion(Vector3 TargetPos);//�v���C���[���E��Motion
    //�`��
    void Draw(void) override;

    //�ʒu��Ԃ�
    Vector3 GetPosition(void);
    //�����Ă���������擾
    Vector3 GetDirectionVec(void);
    Vector3 GetTargetLine(void);
    //�v���C���[�Ƃ̏Փ˃t���O��Ԃ�
    bool GetHitPlayerFlag(void);

    //�ǐՃt���O��Ԃ�
    bool GetHuntFlag(void);

private:
    //�v���C���[��T�����鏈��
    void SearchPlayer(const Vector3 playerPos);

    //�v���C���[�T��(ture:����/false:�������Ă��Ȃ�)
    bool Search(const Vector3 playerPos);

    //�v���C���[��ǐՂ��鏈��
    void HuntPlayer(void);

    //�_�C�N�X�g������
    void DijkstraProcess(Vector3 playerPos,ItemID itemID, bool alphaFlag);

    //�Փˏ���
    bool CheckHitPlayer(void);

    //�t���b�V�����󂯂��Ƃ��̏���
    void DamageFlash(bool hitFlash);

    //�t���b�V����Ԃ���񕜂��鏈��
    void RecoverFlash(void);

    //�X�^�����ԉ�
    void RecoverStanTime(void);

    //�X�^�����ԒZ�k
    void ReduceStanTime(void);

    //�^�[�Q�b�g�̕���������
    void Lock_On_Target(Vector3 targetPos);
    //���f������
    Transform transform_;

    //�A�j���[�V�����R���g���[��
    std::unique_ptr<AnimationController> animController_;

    //�ǐՊ֌W
    bool huntFlag_;         //�v���C���[�����t���O
    double stepHuntTime_;   //�ǐՎ���

    //�ړI�n
    Vector3 goalPos_;
    //�����Ă�������̃x�N�g��
    Vector3 searchDir_;
    //��]
    Quaternion targetRot_;  //�ړI�̕���
    double stepRotTime_;    //��]����

    //�X�^���֌W
    bool stanFlag_;         //�X�^���t���O
    float nowStanTime_;      //�G�̃X�^������(�X�^������قǒZ���Ȃ�)
    double stepStanTime_;   //�X�^������

    //�_�C�N�X�g�����
    std::unique_ptr<Dijkstra> dijkstra_;
    //�M�~�b�N���
    std::vector<std::shared_ptr<Gimmick>> gimmickList_;

    //�v���C���[�̃J�v�Z�����
    Capsule playerCapsule_;

    //�v���C���[�Փ˃t���O
    bool hitPlayerFlag_;

    //�Ǔ��߃t���O(�G���ŗp�ӂ��Ȃ��Ə�ɒǐՃ��[�h�ƂȂ�)
    bool wallAlphaFlag_;

    //�ڐ��̐�
    Vector3 targetPos_;
};
