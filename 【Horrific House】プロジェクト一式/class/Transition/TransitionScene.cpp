#include <DxLib.h>
#include "TransitionScene.h"
#include "../../_debug/_DebugConOut.h"
#include "../../_debug/_DebugDispOut.h"

TransitionScene::TransitionScene(UniqueScene beforScene, UniqueScene afterScene)
{
	// 明示的にキャストする必要がある
	beforScene_ = std::move(beforScene);
	afterScene_ = std::move(afterScene);
	//TRACE("TransitionSceneの生成\n");
	count = 0;
	SetDrawScreen(screenID_);
	ClsDrawScreen();
	beforScene_->Draw();
}

TransitionScene::TransitionScene(UniqueScene afterScene)
{
	// 明示的にキャストする必要がある
	afterScene_ = std::move(afterScene);
	//TRACE("TransitionSceneの生成\n");
	count = 0;
	SetDrawScreen(screenID_);
	ClsDrawScreen();
	afterScene_->Draw();
}

TransitionScene::~TransitionScene()
{
}

void TransitionScene::Init(void)
{
	return ;
}

UniqueScene TransitionScene::Update(UniqueScene ownScene)
{
	if (UpdateTransition())
	{
		//TRACE("次のシーンに移行\n");
		return std::move(afterScene_);
	}
	DrawScreen();
	return ownScene;
}




