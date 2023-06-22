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
    
    int tookTime = GetNowCount() - mStartTime;     //������������ = �o�ߎ��� - ����J�n����
    int waitTime = mCount * 1000 / FPS - tookTime; //�҂ׂ����� = ������ׂ����� - ������������
    if (waitTime > 0)
    {
        Sleep(waitTime);  //�ҋ@
    }
}
