#pragma once
#include<math.h>
#include<DxLib.h>

class Fps
{
private:
	int mStartTime;	//����J�n����
	int mCount;		//�J�E���^
	float mFps;		//FPS
	static const int N = 60;//���ς��Ƃ�T���v����
	static const int  FPS = 60; //�ݒ肵����FPS

public:
	Fps();
	~Fps();

	bool Update();
	void Draw();
	void Wait();

};

