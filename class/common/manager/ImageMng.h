#pragma once
#include<vector>
#include<string>
#include<map>
#include"../Geometry.h"

#define lpImageMng ImageMng::GetInstance()

using VecInt = std::vector<int>;

class ImageMng
{
public:
	static ImageMng& GetInstance()
	{
		static ImageMng s_Instance;
		return s_Instance;
	}

	const VecInt& GetID(std::string key);
	const VecInt& GetID(std::string f_name, std::string key, Vector2Int divSize, Vector2Int divCnt);
	const VecInt& GetID(std::string f_name, std::string key);
private:
	ImageMng();
	~ImageMng();
	std::map<std::string, VecInt> imageMap_;
};

