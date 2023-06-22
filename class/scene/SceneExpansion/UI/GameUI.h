#pragma once
#include "UI.h"
#include "../../../common/manager/TutorialMng.h"




constexpr int CNT_TYPE_MAX = (int)CntType::Max;
constexpr int TUTORIAL_TYPE_MAX = (int)TutorialProgress::MAX;

class GameUI :
    public UI
{
public:
    GameUI();
    GameUI(CntType type);
    ~GameUI();
    void Init()override;
    void Update()override;
    void Draw()override;
private:
    //UIMap
    std::map<TutorialProgress, MoveUIObj> UIVariableMap_;
    //TutorialÇÃêiíªèÛãµ
    TutorialProgress t_progress_;
    int t_progressCounter_;
    CntType cntType_;

    bool moveFlg_;
};

