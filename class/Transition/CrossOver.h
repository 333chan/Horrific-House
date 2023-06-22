#pragma once
#include "TransitionScene.h"

class CrossOver :
    public TransitionScene
{
public:
    CrossOver(double limitTime, UniqueScene beforScene, UniqueScene afterScene);
    ~CrossOver();
private:
    double limitTime_;

    //bool Init(void) override;
    bool UpdateTransition();
    void DrawScreen()override;

};

