#pragma once
#include "Gimmick.h"

class AnimationController;

class InnerLockDoor :
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
    /// <param name="�@���̌���"></param>
    InnerLockDoor(std::string name, Vector3 pos, Vector3 angle, Vector3 scale, int num ,float normal);
    ~InnerLockDoor();

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
    //�ϐ�������
    void VariableInit(void);
    //�A�j���[�V��������
    void Animation(void);

    //�`�掞�Ԍv�Z
    void DrawTimeCount();

    //UI�`��
    void DrawUI();

    //�V�F�[�_�[
    std::unique_ptr<OriginalShader> origShader_;
    //���C�e�B���O
    int psLight_;
    int vsLight_;
    //�w�肵��������������J���邽�߂̖@��
    float normal_;

    //�o�ߎ���
    double delta_;

    //UI�\���t���O
    bool uiFlg_;

    //�摜�T�C�Y�̔����̒l
    Vector2Int graphSize_;

    //�`��ʒu
    Vector2Int drawPos;
    std::string uiKey_;
    //�A�j���[�V�����t���O
    bool animFlg_;

    //���񔻒�p�t���O
    bool firstTimeFlg_;
};

