#include <DxLib.h>
#include <algorithm>
#include <cassert>
#include "../../scene/SceneManager.h"
#include "SoundMng.h"

#include "../../../_debug/_DebugConOut.h"


const VecInt& SoundMng::GetID(std::string key)
{
	if (soundMap_.count(key) == 0)//音を読み込んでいるかどうか
	{
		//読み込んでない場合は読み込む
		soundMap_[key].resize(1);
		soundMap_[key][0] = LoadSoundMem(key.c_str());
		/*assert(soundMap_[key][0] >= 0);*/
		cnt_[key] = 0;
	}
	return soundMap_[key];//音のハンドルを渡す
}

void SoundMng::SetDefaultVolume(void)
{
	for (auto& sound : soundMap_)
	{
		soundVolumeMap_[sound.first] = GetVolumeSoundMem2(sound.second[0]);
	}
}

void SoundMng::PlayingSound(std::string name)
{
	if (CheckSoundMem(lpSoundMng.GetID(name)[0]) == 0)
	{
		PlaySoundMem(lpSoundMng.GetID(name)[0], DX_PLAYTYPE_BACK);
	}
	
}

void SoundMng::PlaySoundOneTime(std::string name)
{
	cnt_[name]++;
	if (cnt_[name] == 1)
	{
		if (CheckSoundMem(lpSoundMng.GetID(name)[0]) == 0)
		{
			
			PlaySoundMem(lpSoundMng.GetID(name)[0], DX_PLAYTYPE_BACK);
		}
	}

}

void SoundMng::ResetCnt(std::string name)
{
	StopSoundMem(lpSoundMng.GetID(name)[0]);
	SetSoundCurrentPosition(0x000000, lpSoundMng.GetID(name)[0]);
	cnt_[name] = 0;
}

SoundMng::SoundMng()
{
}

SoundMng::~SoundMng()
{
}

void SoundMng::ChangeVolume(int Vol, std::string name)
{
	ChangeVolumeSoundMem(Vol, GetID(name)[0]);
}

void SoundMng::AllChangeVolume(int Vol)
{
	for (auto& sound : soundVolumeMap_)
	{
		auto s = sound.second;
		Vol = std::clamp(Vol, -100 + sound.second, 100 + sound.second);
		Vol = std::clamp(Vol,0,255);
		ChangeVolume(Vol, sound.first);
	}
}

void SoundMng::SetAllVolume(int Vol)
{
	allVolume_ = Vol;
}

int SoundMng::GetAllVolume()
{
	return allVolume_;
}

void SoundMng::Play3DSound(std::string path,Vector3 pos,float distance)
{
	// 流れていたら入らない
	if (CheckSoundMem(lpSoundMng.GetID(path)[0]) == 0)
	{
		//音の再生
		/*lpSoundMng.PlayingSound("./resource/sound/test.wav");*/
		PlaySoundMem(lpSoundMng.GetID(path)[0], DX_PLAYTYPE_LOOP);
		//音の座標設定
		Set3DPositionSoundMem(pos.toVECTOR(), lpSoundMng.GetID(path)[0]);
		// サウンドが届く( 音が聞こえる )範囲を設定
		Set3DRadiusSoundMem(distance, lpSoundMng.GetID(path)[0]);
	}
}

void SoundMng::StopSound(std::string name)
{
	StopSoundMem(lpSoundMng.GetID(name)[0]);
}

void SoundMng::AllStopSound()
{
	for (auto& sound : soundMap_)
	{
		StopSoundMem(lpSoundMng.GetID(sound.first)[0]);
	}
}

void SoundMng::ShortCheckStopSound(std::string name, double time)
{
	if (CheckSoundMem(lpSoundMng.GetID(name)[0]))
	{
		playTime_ += lpSceneMng.GetDeltaTime();
	}
	if (playTime_ > time)
	{
		StopSoundMem(lpSoundMng.GetID(name)[0]);
		playTime_ = 0.0;
	}
}
