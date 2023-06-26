#pragma once
#include "CBuffer.h"
class GameOverCBuffer :
    public CBuffer
{
public:
	GameOverCBuffer();
	~GameOverCBuffer();

	void Init(void);
	void Update(void);
	void Update(double time);
private:

	float cnt_;
	double time_;
};

