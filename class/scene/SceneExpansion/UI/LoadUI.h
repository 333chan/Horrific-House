#pragma once
#include "UI.h"
class LoadUI :
    public UI
{
public:
    LoadUI();
    ~LoadUI();
    void Init()override;
    void Update()override;
    void Draw()override;
    //���[�h���I�������
    bool GetIsLoadingEnd();
private:
    bool IsLoadingEnd;  //�i�s�x
    float LoadVol;      //������       
    int maskHandle_;    //�}�X�N�n���h��
};

