#pragma once
#include "Gimmick.h"
class Piano :
    public Gimmick
{
public:
    Piano();
    ~Piano();
    //������
    void Init(void)override;

    //�X�V
    void Update(void)override;

    //�`��
    void Draw(void)override;

    //�J��
    void Release(void)override;

    //���f���̌���Ԃ�
    int GetModelNum(void)override;

    //���f���n���h���̕������Ԃ�
    std::string GetCollModelStr(void)override;
private:
};

