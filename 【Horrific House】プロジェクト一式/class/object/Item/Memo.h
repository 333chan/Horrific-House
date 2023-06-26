#pragma once
#include "Item.h"

class OriginalShader;

class Memo :
    public Item
{
public:
    Memo(std::string name, Vector3 bPos, Vector3 aPos, Vector3 angle, Vector3 scale, ItemID memoID, int num);
    ~Memo();

    //������
    void Init(void)override;

    //�X�V
    void Update(void)override;

    //�`��
    void Draw(void)override;
    void Draw(bool cameraflag);
    //�J��
    void Release(void)override;

    //�A�C�e��ID��Ԃ�
    ItemID GetItemID(void)override { return itemId_; };
private:
    //�ړ�����
    void Move(void);
    //�V�F�[�_
    std::unique_ptr<OriginalShader> origShader_;
    //�ړ����
    ItemMove moveState_;
    //�A�j���[�V�������
    ItemAnim animState_;
};

