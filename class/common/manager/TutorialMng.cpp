#include "TutorialMng.h"

TutorialMng::TutorialMng()
{
}

TutorialMng::~TutorialMng()
{
}

void TutorialMng::InitFlg()
{
	for (int i = 0; i < (int)TutorialProgress::MAX; i++)
	{
		tutorialFlg_[i] = false;
	}
}

void TutorialMng::SetTutorialFlg(TutorialProgress name,bool flg)
{
	tutorialFlg_[(int)name] = flg;
}

bool TutorialMng::GetTutorialFlg(TutorialProgress name)
{
	return tutorialFlg_[(int)name];
}
