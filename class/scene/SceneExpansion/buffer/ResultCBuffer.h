#pragma once
#include "CBuffer.h"
class ResultCBuffer :
    public CBuffer
{
public:

	ResultCBuffer();
	~ResultCBuffer();

	//初期化
	void Init(void);
	//更新
	void Update(float count,Vector3 cameraPos);

private:
};

