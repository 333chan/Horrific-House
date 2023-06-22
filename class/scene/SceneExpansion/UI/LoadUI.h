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
    //ロードが終わったか
    bool GetIsLoadingEnd();
private:
    bool IsLoadingEnd;  //進行度
    float LoadVol;      //増加量       
    int maskHandle_;    //マスクハンドル
};

