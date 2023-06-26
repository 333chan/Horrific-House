#pragma once

#include "../Obj.h"
#include "../../common/Geometry.h"

enum class ItemID
{
    Non,
    Entrance_Key,   //���ւ̌�
    Kitchen_Key,    //�L�b�`���̌�
    MusicRoom_Key,  //���y���̌�
    FoodBank_Key,   //�H�ƌɂ̌�
    ChildRoom_Key,  //�q�������̌�
    Celler_Key,     //�n�����̌�
    Phot,           //�ʐ^
    Mapitem,        //�}�b�v�A�C�e��
    Story_Memo_B,   //�X�g�[���[B�Ɋւ��郁��
    Enemy_Memo,     //�G�Ɋւ��郁��
    Wall_Memo,      //�ǂ̎ʐ^�Ɋւ��郁��
    Picture_Memo,   //�G��Ɋւ��郁��
    CookWare_Memo,  //�������Ɋւ��郁��
    Score_Memo,     //�y���Ɋւ��郁��
    Living_Memo,    //���r���O�Ɋւ��郁��
    Celler_Memo,    //�n�����Ɋւ��郁��
    Dummy_Memo,    //�n�����Ɋւ��郁��
    Story_Book_A,   //�X�g�[���[A�Ɋւ��郁��
    Story_Book_C,   //�X�g�[���[C�Ɋւ��郁��
    Clock_Book,     //���v�Ɋւ��郁��
    Dummy_Book,     //���v�Ɋւ��郁��
    LeftNumber,     //�_�C�����L�[�̍����̃q���g
    RightNumber,    //�E���̃q���g
    Max
};
//�A�C�e���ړ��p
enum class ItemMove
{
    Open,       //�J���Ă��鎞
    Close,      //�܂��Ă��鎞
    Max
};

//�A�C�e���̃A�j���[�V�������
enum class ItemAnim
{
    Playing,    //�Đ�(�ړ�)
    Stop,       //��~
    Max
};

//�A�C�e���I�u�W�F�N�g���
struct ItemObj
{
    std::string objName_;   //�I�u�W�F�N�g��
    Vector3 homePos_;       //���W
    Vector3 movedPos_;      //���W
    Vector3 angle_;	        //��]
    Vector3 scale_;	        //�g��l
    ItemObj(std::string objName, const Vector3& bPos, const Vector3& aPos, const Vector3& angle, const Vector3& scale) :
        objName_(objName),
        homePos_(bPos),
        movedPos_(aPos),
        angle_(angle),
        scale_(scale) {};
};


struct InventoryObj
{
    std::string modelString_;       //���f���n���h��
    int modelNum_;                  //������
    ItemID itemID_;                 //�A�C�e��ID
    Vector3 pos_;                   //���W
    Vector3 scale_;                 //�g��l
    std::string inventoryNum_;      //�C���x���g����
    ItemID usedItemID_;             //�A�C�e��ID
};

class Item
    :public Obj
{
public:
    Item();
    ~Item();
    virtual void Draw(bool cameraflag) {};
   

    
    /// <summary>
    /// ���f���n���h����Ԃ�
    /// </summary>
    /// <param name="void"></param>
    /// <returns></returns>
    virtual std::string GetModelString(void) { return modelString_; }
    
    /// <summary>
    /// �I�u�W�F�N�g����Ԃ�
    /// </summary>
    /// <param name="name_"></param>
    /// <returns></returns>
    virtual std::string GetName(void) { return name_; }

    //�A�C�e��ID��Ԃ�
    virtual ItemID GetItemID(void) = 0;

    //���f���̕����ԍ���Ԃ�
    virtual int GetModelNumber(void) { return modelNum_; }

    //�擾�t���O���Z�b�g
    virtual void SetPickUpFlg(bool flg) { pickupFlg_ = flg; }

    //�擾�t���O��Ԃ�
    virtual bool GetPickUpFlg(void) { return pickupFlg_; }

    //�����蔻��t���O��Ԃ�
    virtual void SetHitFlg(bool flg) { hitFlg_ = flg; }

    //�ړ�����p�t���O��Ԃ�
    virtual void SetMoveFlg(bool flg) { moveFlg_=flg; }

    //���b�N��ԃt���O��Ԃ�
    virtual bool GetLockFlg(void) { return lockFlg_; }


private:
protected:
    //���f���̃t�@�C���p�X
    std::string modelString_;

    //�I�u�W�F�N�g��
    std::string name_;

    //���f��������
    int modelNum_;

    //���C�e�B���O
    int vsLight_;
    int psLight_;

    //���W
    Vector3 pos_;

    //�������W
    Vector3 homePos_;

    //�ړ�����W
    Vector3 movedPos_;

    //�p�x
    Vector3 angle_;

    //�g��l
    Vector3 scale_;

    //�擾�t���O
    bool pickupFlg_;

    //�ړ��t���O
    bool moveFlg_;

    //���b�N�t���O
    bool lockFlg_;

    //�����蔻��p�t���O
    bool hitFlg_;

    //�A�C�e��ID
    ItemID itemId_;
};

