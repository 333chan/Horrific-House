#pragma once
#include<math.h>
#include<DxLib.h>

class Fps
{
private:
	int mStartTime;	//測定開始時刻
	int mCount;		//カウンタ
	float mFps;		//FPS
	static const int N = 60;//平均をとるサンプル数
	static const int  FPS = 60; //設定したいFPS

public:
	Fps();
	~Fps();

	bool Update();
	void Draw();
	void Wait();

};

