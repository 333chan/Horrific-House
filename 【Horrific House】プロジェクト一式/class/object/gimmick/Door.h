#pragma once
#include "Gimmick.h"
class AnimationController;

class Door :
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
    //Door(Vector3 pos,Vector3 angle, std::string name, int num, int collNum);
    Door(std::string name, Vector3 pos, Vector3 angle, Vector3 scale, int num);
    ~Door();

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
    //�A�j���[�V����
    std::unique_ptr<AnimationController> animController_;
    std::unique_ptr<OriginalShader> origShader_;
    //�ϐ�������
    void VariableInit(void);
    //�A�j���[�V��������
    void Animation(void);
    //���C�e�B���O
    int psLight_;
    int vsLight_;
    //�A�j���[�V�����t���O
    bool animFlg_;

};

