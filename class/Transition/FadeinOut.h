#pragma once
#include "TransitionScene.h"
class FadeinOut :
    public TransitionScene
{
public:
    FadeinOut(double limitTime, UniqueScene beforScene, UniqueScene afterScene);
    FadeinOut(double limitTime, Transition tras, UniqueScene afterScene);
    ~FadeinOut();

private:
    double limitTime_;

    bool UpdateTransition()override;
    /*void Draw(double delta);*/
    void DrawScreen()override;
    Transition tras_;
    bool ff = false;
};

