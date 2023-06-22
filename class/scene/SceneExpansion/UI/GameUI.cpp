#include "GameUI.h"

constexpr int SCREEN_SIZE_X = 1920;
constexpr int UI_MOVE_VALUE = 40;
constexpr int MAX_STOP_TIME = 200;
constexpr float UI_CENTER_OFFSET_X = 1500.0f;
constexpr int UI_POP_SPEED = 30;
constexpr int ALPHA_MAX = 255;

GameUI::GameUI()
{
	Init();
}

GameUI::GameUI(CntType type)
{
	cntType_ = type;
	Init();

}

GameUI::~GameUI()
{
}

void GameUI::Init()
{
	lpTutorialMng.InitFlg();
	tmxObj_.LoadTmx("resource/tmx/game.tmx", false);

	t_progress_ = TutorialProgress::None;
	t_progressCounter_ = (int)t_progress_;
	

	switch (cntType_)
	{
	case CntType::Key:
		UIVariableMap_ =
		{
			{TutorialProgress::None,MoveUIObj("key_Tutorial0",{0,0},{0,0},StatusUI::Stop,0,0,false)},
			{TutorialProgress::Move,MoveUIObj("key_Tutorial1",{0,0},{0,0},StatusUI::Stop,0,0,false)},
			{TutorialProgress::ViewShift,MoveUIObj("key_Tutorial2",{0,0},{0,0},StatusUI::Stop,0,0,false)},
			{TutorialProgress::Dash,MoveUIObj("key_Tutorial3",{0,0},{0,0},StatusUI::Stop,0,0,false)},
			{TutorialProgress::Action ,MoveUIObj("key_Tutorial4",{0,0},{0,0},StatusUI::Stop,0,0,false)},
			{TutorialProgress::Inventry,MoveUIObj("key_Tutorial5",{0,0},{0,0},StatusUI::Stop,0,0,false)},
			{TutorialProgress::SetupCamera,MoveUIObj("key_Tutorial6",{0,0},{0,0},StatusUI::Stop,0,0,false)},
			{TutorialProgress::Snap,MoveUIObj("key_Tutorial7",{0,0},{0,0},StatusUI::Stop,0,0,false)},
			{TutorialProgress::Flash,MoveUIObj("key_Tutorial8",{0,0},{0,0},StatusUI::Stop,0,0,false)},

		};
		break;
	case CntType::Pad:
		UIVariableMap_ =
		{
			{TutorialProgress::None,MoveUIObj("pad_Tutorial0",{0,0},{0,0},StatusUI::Stop,0,0,false)},
			{TutorialProgress::Move,MoveUIObj("pad_Tutorial1",{0,0},{0,0},StatusUI::Stop,0,0,false)},
			{TutorialProgress::ViewShift,MoveUIObj("pad_Tutorial2",{0,0},{0,0},StatusUI::Stop,0,0,false)},
			{TutorialProgress::Dash,MoveUIObj("pad_Tutorial3",{0,0},{0,0},StatusUI::Stop,0,0,false)},
			{TutorialProgress::Action ,MoveUIObj("pad_Tutorial4",{0,0},{0,0},StatusUI::Stop,0,0,false)},
			{TutorialProgress::Inventry,MoveUIObj("pad_Tutorial5",{0,0},{0,0},StatusUI::Stop,0,0,false)},
			{TutorialProgress::SetupCamera,MoveUIObj("pad_Tutorial6",{0,0},{0,0},StatusUI::Stop,0,0,false)},
			{TutorialProgress::Snap,MoveUIObj("pad_Tutorial7",{0,0},{0,0},StatusUI::Stop,0,0,false)},
			{TutorialProgress::Flash,MoveUIObj("pad_Tutorial8",{0,0},{0,0},StatusUI::Stop,0,0,false)},
		};
		break;
	case CntType::Smh:
		UIVariableMap_ =
		{
			{TutorialProgress::None,MoveUIObj("smh_Tutorial0",{0,0},{0,0},StatusUI::Stop,0,0,false)},
			{TutorialProgress::Move,MoveUIObj("smh_Tutorial1",{0,0},{0,0},StatusUI::Stop,0,0,false)},
			{TutorialProgress::ViewShift,MoveUIObj("smh_Tutorial2",{0,0},{0,0},StatusUI::Stop,0,0,false)},
			{TutorialProgress::Dash,MoveUIObj("smh_Tutorial3",{0,0},{0,0},StatusUI::Stop,0,0,false)},
			{TutorialProgress::Action ,MoveUIObj("smh_Tutorial4",{0,0},{0,0},StatusUI::Stop,0,0,false)},
			{TutorialProgress::Inventry,MoveUIObj("smh_Tutorial5",{0,0},{0,0},StatusUI::Stop,0,0,false)},
			{TutorialProgress::SetupCamera,MoveUIObj("smh_Tutorial6",{0,0},{0,0},StatusUI::Stop,0,0,false)},
			{TutorialProgress::Snap,MoveUIObj("smh_Tutorial7",{0,0},{0,0},StatusUI::Stop,0,0,false)},
			{TutorialProgress::Flash,MoveUIObj("smh_Tutorial8",{0,0},{0,0},StatusUI::Stop,0,0,false)},
		};
		break;
	case CntType::Max:
		break;
	default:
		break;
	}
	//tmxから取ってきた値をUIMapに代入する
	for (auto& coll : tmxObj_.GetTipsCollList())
	{
		for (auto& ui : UIVariableMap_)
		{
			if (coll.first == ui.second.imageName)
			{
				Vector2 cen = { coll.second.first.x + coll.second.second.x / 2 ,coll.second.first.y + coll.second.second.y / 2 };
																		  //目的地				//現在の位置			     移動中か        //値
				UIVariableMap_[ui.first] = MoveUIObj(ui.second.imageName,Vector2(cen.x, cen.y), Vector2(cen.x + UI_CENTER_OFFSET_X, cen.y), StatusUI::Stop, 0, 0, false);
			}
		}
	}
}

void GameUI::Update()
{
	Vector2Int GraphSize;
	for (auto& ui : UIVariableMap_)
	{
		if (ui.first == (TutorialProgress)t_progressCounter_)
		{
			if (lpTutorialMng.GetTutorialFlg((TutorialProgress)t_progressCounter_) && !moveFlg_ || ui.first == TutorialProgress::None)
			{
				t_progressCounter_++;
				continue;
			}
			if (!lpTutorialMng.GetTutorialFlg((TutorialProgress)t_progressCounter_) && !moveFlg_)
			{
				ui.second.IsMove = StatusUI::MoveIn;
				if (ui.first == TutorialProgress::SetupCamera)lpTutorialMng.SetTutorialFlg(TutorialProgress::SetupCamera, true);
				moveFlg_ = true;
			}
			if (ui.second.endflag && lpTutorialMng.GetTutorialFlg((TutorialProgress)t_progressCounter_)
				||ui.second.endflag && (TutorialProgress)t_progressCounter_ == TutorialProgress::SetupCamera)
			{
				moveFlg_ = false;
				t_progressCounter_++;
			}
		}
	}

	for (auto& ui : UIVariableMap_)
	{
		GetGraphSize(lpImageMng.GetID(ui.second.imageName)[0], &GraphSize.x, &GraphSize.y);
		GraphSize.x /= 2;
		GraphSize.y /= 2;

		if (ui.second.StopCnt < 0)
		{
			continue;
		}
		if (ui.second.IsMove == StatusUI::MoveIn && !ui.second.endflag)
		{
			if (ui.second.Destination.x < ui.second.Pos.x || ui.second.Pos.x + GraphSize.x > SCREEN_SIZE_X)
			{
				ui.second.Pos.x -= UI_MOVE_VALUE;
			}
			else
			{
				ui.second.StopCnt++;

				if (ui.second.StopCnt > MAX_STOP_TIME && lpTutorialMng.GetTutorialFlg((TutorialProgress)t_progressCounter_))
				{
					ui.second.IsMove = StatusUI::MoveOut;
				}
			}
			if (ui.second.popCnt < ALPHA_MAX)
			{
				ui.second.popCnt += 3;
			}
			else
			{
				ui.second.popCnt = ALPHA_MAX;
			}
		}
		if (ui.second.IsMove == StatusUI::MoveOut && !ui.second.endflag)
		{
			if (ui.second.Pos.x >= SCREEN_SIZE_X+GraphSize.x)
			{
				ui.second.Pos.x = SCREEN_SIZE_X+GraphSize.x;
				ui.second.StopCnt = 0;
				ui.second.popCnt = 0;
				ui.second.endflag = true;
			}
			else
			{
				ui.second.Pos.x += UI_MOVE_VALUE;
				if (ui.second.popCnt >0)
				{
					ui.second.popCnt -= UI_POP_SPEED;
				}
				else
				{
					ui.second.popCnt = 0;
				}
			}
		}
		
	}
}

void GameUI::Draw()
{
	Vector2Int GraphSize;

	for (auto& ui : UIVariableMap_)
	{
		GetGraphSize(lpImageMng.GetID(ui.second.imageName)[0], &GraphSize.x, &GraphSize.y);
		GraphSize.x /= 2;
		GraphSize.y /= 2;
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, ui.second.popCnt);
		//DrawBox(static_cast<int>(ui.second.Pos.x) - GraphSize.x, static_cast<int>(ui.second.Pos.y) - GraphSize.y, static_cast<int>(ui.second.Pos.x) + GraphSize.x, static_cast<int>(ui.second.Pos.y) + GraphSize.y, 0x222222, true);
		DrawGraph(static_cast<int>(ui.second.Pos.x) - GraphSize.x, static_cast<int>(ui.second.Pos.y) - GraphSize.y, lpImageMng.GetID(ui.second.imageName)[0], true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
			
	}
	
}
