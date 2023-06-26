#pragma once
#include "CBuffer.h"
class ResultCBuffer :
    public CBuffer
{
public:

	ResultCBuffer();
	~ResultCBuffer();

	//‰Šú‰»
	void Init(void);
	//XV
	void Update(float count,Vector3 cameraPos);

private:
};

