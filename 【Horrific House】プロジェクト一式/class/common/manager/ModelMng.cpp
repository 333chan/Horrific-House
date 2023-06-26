#include <DxLib.h>
#include "ModelMng.h"
#include <cassert>

const VecInt& ModelMng::GetID(std::string key)
{
	if (!ModelMap_.count(key))
	{
		//読み込んでいない場合リサイズして画像をロードする
		ModelMap_[key].resize(1);
		ModelMap_[key][0] = MV1LoadModel(key.c_str());
		//assert(ModelMap_[key][0] >= 0);
	}
	
	return ModelMap_[key];
}


const VecInt& ModelMng::GetID(std::string key,int t)
{
	//画像を読み込んでいるかのチェック
	if (!ModelMap_.count(key))
	{
		//読み込んでいない場合リサイズして画像をロードする
		ModelMap_[key].resize(t);
		ModelMap_[key][0] = MV1LoadModel(key.c_str());
		for (int i = 1; i < t; i++)
		{
			ModelMap_[key][i] = MV1DuplicateModel(ModelMap_[key][0]);
		}
	}
	
	return ModelMap_[key];
}

void ModelMng::Draw(int handle, VECTOR pos, VECTOR rot, VECTOR scale)
{
	MV1SetPosition(handle, pos);
	MV1SetRotationXYZ(handle, rot);
	MV1SetScale(handle, scale);
	MV1DrawModel(handle);
}

ModelMng::ModelMng()
{
}

ModelMng::~ModelMng()
{
}
