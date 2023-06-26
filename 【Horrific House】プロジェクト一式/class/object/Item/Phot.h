#pragma once
#include "Item.h"

class OriginalShader;

class Phot
    : public Item
{
public:
    Phot(std::string name, Vector3 bPos, Vector3 aPos, Vector3 angle, Vector3 scale, ItemID id, int num);
    ~Phot();

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
    ItemID GetItemID(void)override { return ItemID::Phot; };

private:
    std::unique_ptr<OriginalShader> origShader_;

protected:

};

