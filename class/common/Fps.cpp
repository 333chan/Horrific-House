#include "Fps.h"
#include "../../_debug/_DebugConOut.h"

Fps::Fps()
{
    mStartTime = 0;
    mCount = 0;
    mFps = 0;
}

Fps::~Fps()
{
}

bool Fps::Update()
{
    if (mCount == 0)
    {
        mStartTime = GetNowCount();
    }
    if (mCount == N)
    {
        int t = GetNowCount();
        mFps = 1000.f / ((t - mStartTime) / (float)N);
        mCount = 0;
        mStartTime = t;
    }
    mCount++;
    return true;
}

void Fps::Draw()
{
    TRACE("FPS = %.1f\n", mFps);
    //TRACE("FPS  %f\n", GetFPS());
}

void Fps::Wait()
{
    
    int tookTime = GetNowCount() - mStartTime;     //かかった時間 = 経過時間 - 測定開始時刻
    int waitTime = mCount * 1000 / FPS - tookTime; //待つべき時間 = かかるべき時間 - かかった時間
    if (waitTime > 0)
    {
        Sleep(waitTime);  //待機
    }
}
