#include <DxLib.h>
#include "TransitionScene.h"
#include "../../_debug/_DebugConOut.h"
#include "../../_debug/_DebugDispOut.h"

TransitionScene::TransitionScene(UniqueScene beforScene, UniqueScene afterScene)
{
	// �����I�ɃL���X�g����K�v������
	beforScene_ = std::move(beforScene);
	afterScene_ = std::move(afterScene);
	//TRACE("TransitionScene�̐���\n");
	count = 0;
	SetDrawScreen(screenID_);
	ClsDrawScreen();
	beforScene_->Draw();
}

TransitionScene::TransitionScene(UniqueScene afterScene)
{
	// �����I�ɃL���X�g����K�v������
	afterScene_ = std::move(afterScene);
	//TRACE("TransitionScene�̐���\n");
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
		//TRACE("���̃V�[���Ɉڍs\n");
		return std::move(afterScene_);
	}
	DrawScreen();
	return ownScene;
}




