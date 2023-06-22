#pragma once
#include <vector>
#include "Gimmick.h"

class AnimationController;

//���Έʒu
enum class RELATIVE_LOC {
    LEFT_UP,
    RIGHT_UP,
    LEFT_DOWN,
    RIGHT_DOWN
};

class Drawer :
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
    Drawer(std::string name, Vector3 pos, Vector3 angle, Vector3 scale, int num);
    ~Drawer();

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

    //���f���̃n���h���̕������Ԃ�
    std::string GetCollModelStr(void)override;

private:
    //�A�j���[�V����
    std::unique_ptr<AnimationController> animController_;
    //�V�F�[�_�[
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

