#pragma once
#include "Gimmick.h"
class FakeWall :
    public Gimmick
{
public:
    /// <summary>
    /// �R���X�g���N�^
    /// </summary>
    /// <param name="�������W"></param>
    /// <param name="�����p�x"></param>
    /// <param name="�I�u�W�F�N�g��"></param>
    /// <param name="������"></param>
    FakeWall(std::string name, Vector3 pos, Vector3 angle, Vector3 scale, int num,ItemID itemID);
    ~FakeWall();

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
