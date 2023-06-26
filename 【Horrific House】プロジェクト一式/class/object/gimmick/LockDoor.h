#pragma once
#include "Gimmick.h"

class AnimationController;
class LockDoor :
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
    /// <param name="�������邽�߂ɕK�v�ȃA�C�e����"></param>
    //LockDoor(Vector3 pos, Vector3 angle, std::string name, int num,int collNum,ItemID itemID);
    LockDoor(std::string name, Vector3 pos, Vector3 angle, Vector3 scale, int num,ItemID itemID);
    ~LockDoor();

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

    //UI�`�掞�Ԍv�Z
    void DrawTimeCount(void);

    void SetGraphSize(void);

    //UI�`��
    void DrawUI(void);

    //UI�摜���ʗp
    std::string uiKey_;

    //�摜�T�C�Y�̔����̒l
    Vector2Int graphSize_;

    //�`��ʒu
    Vector2Int drawPos;

    //���C�e�B���O
    int psLight_;
    int vsLight_;

    //�o�ߎ���
    double delta_;

    //����m�F�p�t���O
    bool firstTimeFlg_;

    //UI�\������p�t���O
    bool uiFlg_;

    //�A�j���[�V�����Đ��t���O
    bool animFlg_ = false;

};

