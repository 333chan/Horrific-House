#pragma once
#include<map>
#include <DxLib.h>
#include <string>

#define lpTutorialMng TutorialMng::GetInstance()

//TutorialÇÃêiíªèÛãµ
enum class TutorialProgress
{
    None,
    Move,
    ViewShift,
    Dash,
    Action,
    Inventry,
    SetupCamera,
    Snap,
    Flash,
    MAX
};

class TutorialMng
{
public:
	static TutorialMng& GetInstance()
	{
		static TutorialMng s_Instance;
		return s_Instance;
	}

    void InitFlg();
    void SetTutorialFlg(TutorialProgress name,bool flg);
    bool GetTutorialFlg(TutorialProgress name);
private:
	TutorialMng();
	~TutorialMng();

    bool tutorialFlg_[(int)TutorialProgress::MAX];
};

