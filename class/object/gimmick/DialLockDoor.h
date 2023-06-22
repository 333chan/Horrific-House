#pragma once
#include "Gimmick.h"
class AnimationController;
class DialLockDoor :
    public Gimmick
{
public:
    /// <summary>
    /// 
    /// </summary>
    /// <param name="�������W"></param>
    /// <param name="�����p�x"></param>
    /// <param name="�I�u�W�F�N�g��"></param>
    /// <param name="������"></param>
    /// <param name="�����ԍ�"></param>
    DialLockDoor(std::string name, Vector3 pos, Vector3 angle, Vector3 scale, int num,std::string unlockNum);
    ~DialLockDoor();

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

    //�V�F�[�_�[
    std::unique_ptr<OriginalShader> origShader_;

    //�ϐ�������
    void VariableInit(void);

    //�A�j���[�V��������
    void Animation(void);

    //UI�\�����ԏ���
    void DrawTimeCount(void);

    //UI�`��
    void DrawUI(void);

    //�摜�T�C�Y�̔����̒l
    Vector2Int graphSize_;

    //�`��ʒu
    Vector2Int drawPos;

    //���C�e�B���O
    int psLight_;
    int vsLight_;

    //�o�ߎ���
    double delta_;

    //�A�j���[�V�����t���O
    bool animFlg_;

    //UI�\���t���O
    bool uiFlg_;

    //���񔻒�p�t���O
    bool firstTimeFlg_;
};

