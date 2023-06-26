#pragma once
#include "Item.h"

class OriginalShader;

class Key
	: public Item
{
public:
    Key(std::string name,Vector3 bPos, Vector3 aPos, Vector3 angle,Vector3 scale,ItemID keyID, int num);
    ~Key();

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
    void Move(void);
    //�V�F�[�_
    std::unique_ptr<OriginalShader> origShader_;
    //�ړ����
    ItemMove moveState_;
    //�A�j���[�V�������
    ItemAnim animState_;
    //�T�C���J�[�u�̃J�E���g
    float count_;
};

