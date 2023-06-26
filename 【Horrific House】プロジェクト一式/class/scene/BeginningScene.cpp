#include "BeginningScene.h"
#include "PreTitleScene.h"
#include "../Transition/FadeinOut.h"
#include "SceneManager.h"
#include "../common/manager/ImageMng.h"

constexpr float FADE_OUT_TIME = 2.0f;
constexpr float FADE_IN_OUT_TIME = 3.0f;

constexpr float DRAW_TIME_LOGO = 10.0f;
constexpr float DRAW_TIME_WARNING = 30.0f;
constexpr float DELTA_TIME_MUL = 10.0f;
constexpr float IMAGE_SCALE = 1.0f;
BeginningScene::BeginningScene()
{
	Init();
	updateFuncPtr_ = beginningTypeMap_[beginningType::Logo].first;
	drawFuncPtr_ = beginningTypeMap_[beginningType::Logo].second;

	DrawScreen();
}
BeginningScene::BeginningScene(beginningType type)
{
	Init();
	updateFuncPtr_ = beginningTypeMap_[type].first;
	drawFuncPtr_ = beginningTypeMap_[type].second;
	
	DrawScreen();
}

BeginningScene::~BeginningScene()
{
	Release();
}

void BeginningScene::Init()
{
	beginningTypeMap_ =
	{
		{beginningType::Logo,{&BeginningScene::UpdateLogo ,&BeginningScene::DrawLogo}},
		{beginningType::Warning,{&BeginningScene::UpdateWarning ,&BeginningScene::DrawWarning}}
	};
	elapsedTime_ = 0.0;
	Loading();
}

UniqueScene BeginningScene::Update(UniqueScene scene)
{
	
	elapsedTime_ += lpSceneMng.GetDeltaTime();
	afterScene_ = beginningType::Non;
	if (updateFuncPtr_ != nullptr)
	{
		(this->*updateFuncPtr_)();
	}
	
	DrawScreen();

	if (afterScene_ == beginningType::Logo)
	{
		return std::make_unique<FadeinOut>(FADE_OUT_TIME, std::move(scene),std::make_unique<BeginningScene>(beginningType::Warning));
	}
	else if(afterScene_ == beginningType::Warning)
	{
		return std::make_unique<FadeinOut>(FADE_IN_OUT_TIME, std::move(scene), std::make_unique<PreTitleScene>());
	}
	return scene;
}

void BeginningScene::DrawScreen(void)
{
	SetDrawScreen(screenID_);
	ClsDrawScreen();
	//ŠÖ”ƒ|ƒCƒ“ƒ^‚ÌŽÀ‘•
	if (drawFuncPtr_ != nullptr)
	{
		(this->*drawFuncPtr_)();
	}
}

void BeginningScene::Release(void)
{
}

void BeginningScene::UpdateLogo()
{

	if (elapsedTime_* DELTA_TIME_MUL > DRAW_TIME_LOGO)
	{
		afterScene_ = beginningType::Logo;
	}
}

void BeginningScene::UpdateWarning()
{
	if (elapsedTime_ * DELTA_TIME_MUL > DRAW_TIME_WARNING)
	{
		afterScene_ = beginningType::Warning;
	}
}

void BeginningScene::DrawLogo()
{
	DrawRotaGraph(lpSceneMng.GetScreenSize().x / 2, lpSceneMng.GetScreenSize().y / 2, IMAGE_SCALE, 0, lpImageMng.GetID("teamLogo")[0], true);
}

void BeginningScene::DrawWarning()
{
	DrawRotaGraph(lpSceneMng.GetScreenSize().x / 2, lpSceneMng.GetScreenSize().y / 2, IMAGE_SCALE, 0, lpImageMng.GetID("warningImage")[0], true);
}
