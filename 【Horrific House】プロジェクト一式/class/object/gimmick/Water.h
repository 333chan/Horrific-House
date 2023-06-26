#pragma once
#include "Gimmick.h"
class Water :
    public Gimmick
{
public:
    Water(std::string name, Vector3 pos, Vector3 angle, Vector3 scale, int num);
    ~Water();
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
    //�V�F�[�_�[
    std::unique_ptr<OriginalShader> origShader_;
    //�ϐ�������
    void VariableInit(void);

};

