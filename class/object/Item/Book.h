#pragma once
#include "Item.h"

class OriginalShader;

class Book
    : public Item
{
public:
    Book(std::string name, Vector3 bPos, Vector3 aPos, Vector3 angle, Vector3 scale, ItemID bookID, int num);
    ~Book();

    //������
    void Init(void)override;

    //�X�V
    void Update(void)override;

    //�`��
    void Draw(void)override;
    void Draw(bool cameraflag);
    //�J��
    void Release(void)override;

    ItemID GetItemID(void)override { return itemId_; };
private:
    //�ړ�
    void Move(void);
    //�V�F�[�_
    std::unique_ptr<OriginalShader> origShader_;
    //�ړ����
    ItemMove moveState_;
    //�A�j���[�V�������
    ItemAnim animState_;
};
