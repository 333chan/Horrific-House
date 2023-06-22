#pragma once
#include <vector>
#include<string>
#include <map>
#include "../../common/Geometry.h"
#define lpMovieMng MovieMng::GetInstance()

using VecInt = std::vector<int>;
class MovieMng
{
public:
	static MovieMng& GetInstance()
	{
		static MovieMng s_Instance;
		return s_Instance;
	}

	const VecInt& GetID(std::string key);
	const VecInt& GetID(std::string f_name, std::string key);
	//‰¹‚ð–Â‚ç‚·
	void PlayOnMovie(std::string name);
	void DrawMovie(std::string name);
	void StopDrawMovie(std::string name);
	void DrawLoopMovie(std::string name);
private:
	MovieMng();
	~MovieMng();

	std::map<std::string, VecInt> MovieMap_;
};

