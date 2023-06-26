#include <DxLib.h>
#include "MovieMng.h"
#include <cassert>

const VecInt& MovieMng::GetID(std::string key)
{
	//�摜��ǂݍ���ł��邩�̃`�F�b�N
	if (!MovieMap_.count(key))
	{
		//�ǂݍ���ł��Ȃ��ꍇ���T�C�Y���ĉ摜�����[�h����
		MovieMap_[key].resize(1);
		MovieMap_[key][0] = LoadGraph(key.c_str());
		assert(MovieMap_[key][0] >= 0);
	}
	return MovieMap_[key];
}

const VecInt& MovieMng::GetID(std::string f_name, std::string key)
{
	if (!MovieMap_.count(key))
	{
		//�ǂݍ���ł��Ȃ��ꍇ���T�C�Y���ĉ摜�����[�h����
		MovieMap_[key].resize(1);
		MovieMap_[key][0] = LoadGraph(f_name.c_str());
	}
	return MovieMap_[key];
}

void MovieMng::PlayOnMovie(std::string name)
{
	PlayMovieToGraph(MovieMap_[name][0]);
	//SetPlaySpeedRateMovieToGraph(MovieMap_[name][0], 0.7);

}

void MovieMng::DrawMovie(std::string name)
{
	if (GetMovieStateToGraph(MovieMap_[name][0]))
	{
		DrawExtendGraph(0, 0, 640, 480, MovieMap_[name][0], FALSE);
		WaitTimer(17);

	}
	else if (TellMovieToGraph(MovieMap_[name][0]))
	{
		SeekMovieToGraph(MovieMap_[name][0], 0);
	}
}

void MovieMng::StopDrawMovie(std::string name)
{
	PauseMovieToGraph(MovieMap_[name][0]);
	/*SeekMovieToGraph(MovieMap_[name][0], 0);*/
}

void MovieMng::DrawLoopMovie(std::string name)
{
	if (GetMovieStateToGraph(MovieMap_[name][0]))
	{
		DrawExtendGraph(0, 0, 640, 480, MovieMap_[name][0], FALSE);
		WaitTimer(17);

	}
	else if (TellMovieToGraph(MovieMap_[name][0]))
	{
		SeekMovieToGraph(MovieMap_[name][0], 0);
		PlayMovieToGraph(MovieMap_[name][0]);
	}
}

MovieMng::MovieMng()
{
}

MovieMng::~MovieMng()
{
}
