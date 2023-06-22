#pragma once
#include "UI.h"

enum class OpeningColUI
{
    Non,
    First,//��ڂ̕�
    Second,//��ڂ̕�
    Third,//�O�ڂ̕�
    Forth,//�l�ڂ̕�
    Five,//�܂ڂ̕�
    Six,//�Z�ڂ̕�
    Seven,//���ڂ̕�
    Max
};

class Controller;
enum class ControllerType;

class OpeningUI :
    public UI
{
public:
    OpeningUI();
    OpeningUI(CntType cntType);
    ~OpeningUI();
    void Init()override;
    void Update()override;
    void Draw()override;
    //�I�[�v�j���O���I�������
    bool GetIsEndFlg();
    int Getprogress();
private:
    //UIMap�̏�����
    void InitUIMap();
    //�R���g���[�����
    std::unique_ptr<Controller> controller_;
    CntType cntType_;
    //UIMap
    std::map<OpeningColUI, UIObj<OpeningColUI>> openingUIMap_;

    double time_;//�o�ߎ���
    int progress_;//�i���x
    bool isEndFlg;//�\�����ԏI���t���O
};

