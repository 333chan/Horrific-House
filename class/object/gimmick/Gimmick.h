#pragma once
#include "../Obj.h"
#include "../../common/Geometry.h"
#include "../../common/manager/ModelMng.h"
#include "../../common/manager/ImageMng.h"

//�M�~�b�N�A�j���[�V�������
enum class GIMMICK_ANIM {
    NONE,
    OPEN,   //�J����
    CLOSE,  //����
    MAX,
};

enum class GIMMICK_TYPE {
    NONE,
    DOOR,       //��
    FRAME,      //�I�u�W�F�N�g�̘g
    DRAWER,     //�����o��
    DIAL,       //�_�C����
    DL_DOOR,    //�_�C�����Ń��b�N����Ă����
    TEXT,       //����
    WALL,       //��
    WATER,      //��
    JUKE_BOX,   //�W���[�N�{�b�N�X
    MAX
};

//�M�~�b�N�I�u�W�F�N�g���
struct GimmickObj
{
    std::string objName_;   //�I�u�W�F�N�g��
    Vector3 pos_;           //���W
    Vector3 angle_;	        //��]
    Vector3 scale_;	        //�g��l
    GimmickObj(std::string objName,const Vector3& pos, const Vector3& angle, const Vector3& scale) :
        objName_(objName),
        pos_(pos),
        angle_(angle),
        scale_(scale) {}
};

//�A�C�e���̎��
enum class ItemID;

class OriginalShader;

class Gimmick :
    public Obj
{
public:
    Gimmick();
    ~Gimmick();
    
    /// <summary>
    /// ���W��Ԃ�
    /// </summary>
    /// <param name=""></param>
    /// <returns></returns>
    virtual Vector3 GetPos(void) { return pos_; }

    /// <summary>
    /// ���f���̐���Ԃ�
    /// </summary>
    /// <param name="void"></param>
    /// <returns></returns>
    virtual int GetModelNum(void) = 0;

    /// <summary>
    /// ���f���n���h���̕������Ԃ�
    /// </summary>
    /// <param name=""></param>
    /// <returns></returns>
    virtual std::string GetCollModelStr(void) = 0;

    /// <summary>
    /// �I�u�W�F�N�g����Ԃ�
    /// </summary>
    /// <param name="void"></param>
    /// <returns></returns>
    virtual std::string GetName(void) { return name_; }

    /// <summary>
    /// ���b�N��Ԃ�Ԃ�
    /// </summary>
    /// <param name=""></param>
    /// <returns></returns>
    virtual bool GetIsLockFlg(void) { return isLock_; }

    /// <summary>
    /// ���b�N��Ԃ��Z�b�g
    /// </summary>
    /// <param name="lockFlg"></param>
    virtual void SetIsLockFlg(bool lockFlg) { isLock_ = lockFlg; }

    /// <summary>
    /// �C���^���N�g�t���O�Z�b�g
    /// </summary>
    /// <param name="�Đ��t���O"></param>
    virtual void SetInteractFlg(bool interactFlg) { interactFlg_ = interactFlg; }
    virtual bool GetInteractFlg(void) { return interactFlg_; }
    virtual void SetHitFlg(bool hitFlg) { hitFlg_ = hitFlg; }

    /// <summary>
    /// �����������̖@����ݒ�
    /// </summary>
    /// <param name="hitNormal"></param>
    virtual void SetHitNormal(VECTOR hitNormal) { hitNormal_ = hitNormal; }

    /// <summary>
    /// �����J���邽�߂ɕK�v�Ȍ�ID��Ԃ�
    /// </summary>
    /// <returns></returns>
    virtual ItemID GetItemID() { return itemID_; };

    /// <summary>
    /// �_�C�A���ԍ���Ԃ�
    /// </summary>
    /// <param name=""></param>
    /// <returns></returns>
    virtual int GetDialNumber(void) { return dialNumber_; }

    /// <summary>
    /// �����ԍ���Ԃ�
    /// </summary>
    /// <param name=""></param>
    /// <returns></returns>
    virtual std::string GetUnlockNumber(void) { return unlockNum_;  }

    /// <summary>
    /// �M�~�b�N�^�C�v��Ԃ�
    /// </summary>
    /// <param name=""></param>
    /// <returns></returns>
    virtual GIMMICK_TYPE GetType(void) { return type_; }

    /// <summary>
    /// �A�j���[�V������Ԃ�Ԃ�
    /// </summary>
    /// <param name=""></param>
    /// <returns></returns>
    virtual GIMMICK_ANIM GetAnimState_(void) { return animState_; }
private:
protected:
    //�I�u�W�F�N�g��
    std::string name_;

     //���W
    Vector3 pos_;

   //�g��l
    Vector3 scale_;

    //��]
    Vector3 angle_;

    //���f��������
    int modelNum_;

    //�_�C�A���̐���
    int dialNumber_;

    //���b�N���
    bool isLock_;

    //�C���^���N�g�t���O
    bool interactFlg_;

    // �����Đ��t���O
    bool soundFlg_;

    //�����蔻��t���O
    bool hitFlg_;

    //���f���̃t�@�C���p�X
    std::string modelStr_;

    //���b�N�����̔ԍ�
    std::string unlockNum_;

    //���b�N����Ă�������J���邽�߂ɕK�v�Ȍ���ID
    ItemID itemID_;

    //�����������̖@��
    VECTOR hitNormal_;

    //�M�~�b�N�̎��
    GIMMICK_TYPE type_;

    //�A�j���[�V�������
    GIMMICK_ANIM animState_;
};