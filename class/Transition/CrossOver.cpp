#include <DxLib.h>
#include "CrossOver.h"
#include "../scene/SceneManager.h"
CrossOver::CrossOver(double limitTime, UniqueScene beforScene, UniqueScene afterScene):
	TransitionScene(std::move(beforScene), std::move(afterScene))
{
	limitTime_ = limitTime;
	//DrawOwnScreen(0.0);
}

CrossOver::~CrossOver()
{
}

bool CrossOver::UpdateTransition()
{
	count += lpSceneMng.GetDeltaTime();
	if (count > limitTime_)
	{
		//I‚í‚Á‚Ä‚¢‚é
		return true;
	}
	//I‚í‚Á‚Ä‚¢‚È‚¢
	return false;
}

void CrossOver::DrawScreen()
{
	SetDrawScreen(screenID_);
	ClsDrawScreen();

	auto alpha = static_cast<int>(255.0 * count / limitTime_);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 - alpha);
	beforScene_->Draw();
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
	afterScene_->Draw();
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, alpha);
}


