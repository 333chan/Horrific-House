#pragma once
#include<vector>
#include<string>
#include<map>
#include "../Geometry.h"

#define lpModelMng ModelMng::GetInstance()

using VecInt = std::vector<int>;



class ModelMng
{
public:
	static ModelMng& GetInstance()
	{
		static ModelMng s_Instance;
		return s_Instance;
	}

	const VecInt& GetID(std::string key);
	const VecInt& GetID(std::string key,int t);
	void Draw(int handle,VECTOR pos,VECTOR rot,VECTOR scale);
private:
	ModelMng();
	~ModelMng();
	std::map<std::string, VecInt> ModelMap_;
};

