#include<DxLib.h>
#include<algorithm>
#include "ImageMng.h"
#include"../../../_debug/_DebugConOut.h"
#include <cassert>
const VecInt& ImageMng::GetID(std::string key)
{
	//画像を読み込んでいるかのチェック
	if (!imageMap_.count(key))
	{
		//読み込んでいない場合リサイズして画像をロードする
		imageMap_[key].resize(1);
		imageMap_[key][0] = LoadGraph(key.c_str());
		//assert(imageMap_[key][0] >= 0);
	}
	return imageMap_[key];
}

const VecInt& ImageMng::GetID(std::string f_name, std::string key, Vector2Int divSize, Vector2Int divCnt)
{
	if (!imageMap_.count(key))
	{
		imageMap_[key].resize(static_cast<UINT>(divCnt.x * divCnt.y));	//static_cast<UINT>

		if (LoadDivGraph(f_name.c_str(),
			divCnt.x * divCnt.y,
			divCnt.x, divCnt.y,
			divSize.x, divSize.y,
			&imageMap_[key][0],
			true) == -1)
		{
			TRACE("失敗");
		}
	}
	return imageMap_[key];

}

const VecInt& ImageMng::GetID(std::string f_name, std::string key)
{
	if (!imageMap_.count(key))
	{
		//読み込んでいない場合リサイズして画像をロードする
		imageMap_[key].resize(1);
		imageMap_[key][0] = LoadGraph(f_name.c_str());
	}
	return imageMap_[key];
}

ImageMng::ImageMng()
{
}

ImageMng::~ImageMng()
{
}
