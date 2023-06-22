#pragma once
#include "CBuffer.h"

struct TitleLight
{
	//DirectionalLight directionalLights;
	SpotLight        SpotLights;
};

class TitleCBuffer:
	public CBuffer
{
public:
	TitleCBuffer(Vector3 cameraPos);
	~TitleCBuffer();

	void Init(Vector3 cameraPos);
	void Update();
private:
	TitleLight titleLight_;
	Vector3 cameraPos_;

	double lightAngle_;
	double StartCnt_;
	double lightPower_;
};

