#pragma once
#include<memory>

class Controller;
enum class ControllerType;

class Stamina
{
public:
    Stamina(ControllerType type);
    ~Stamina();

    //������
    void Init(void);
    //�j��
    void Release(void);
    //�W���̌v�Z�ƌo�ߎ��ԏ���
    float Update(bool isCamera);
    //�`��
    void Draw(void);

private:
    //�ړ��W���ƃ_�b�V�����Ԃ̌v�Z
    void CalcCoefficientAndTime(void);

    //�X�^�~�i�Q�[�W�̃A���t�@�l����
    void CalcStaminaGaugeAlpha(bool transFlag);

    //�X�^�~�i�Q�[�W�̐F�ύX����
    void ChangeStaminaGaugeColor(void);

    //�R���g���[�����
    std::unique_ptr<Controller> controller_;

    //�ړ��W��
    float moveCoefficient_;

    //�_�b�V������
    double dashTime_;

    //�A���t�@�l
    int gaugeAlpha_;

    //�Q�[�W�̐F
    int gaugeColor;

    //�_�b�V���̐؂�ւ��t���O
    bool dashFlag_;
    bool isCamera_;
};

