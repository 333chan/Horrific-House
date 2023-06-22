#include "Goal.h"
#include "../common/manager/ModelMng.h"

Goal::Goal(std::string path, Vector3 pos)
{
	path_ = path;
	pos_ = pos;
	Init();
}

Goal::~Goal()
{
}

void Goal::Init(void)
{
	MV1SetPosition(lpModelMng.GetID(path_)[0], pos_.toVECTOR());
	MV1SetScale(lpModelMng.GetID(path_)[0], { 5,5,5 });
	MV1SetRotationXYZ(lpModelMng.GetID(path_)[0], {0,Deg2RadF(90.0f),0});
}

void Goal::Release(void)
{
}

void Goal::Update(void)
{
}

void Goal::Draw(void)
{
	MV1DrawModel(lpModelMng.GetID(path_)[0]);
}
