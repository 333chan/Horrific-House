#include<DxLib.h>
#include "MouseIcon.h"
#include"../common/manager/ImageMng.h"
#include"../scene/SceneManager.h"
#include"../Input/KeyInput.h"
#include"../Input/PadInput.h"

constexpr int LOWER_LIMIT = 200;		//傾きの最低数値は２００

//感度
constexpr float DIV_SENSITIVITY = 40.0f;

//画像サイズ
constexpr float GRAPH_SIZE = 16.0f;

//オフセット
constexpr int MOUSEICON_OFFSET_X = 4;
constexpr int MOUSEICON_OFFSET_Y = 15;

MouseIcon::MouseIcon()
{
	Init();
}

MouseIcon::~MouseIcon()
{
	Release();
}


void MouseIcon::Init(void)
{
	sceneID_ = SceneID::Title;
	if (GetJoypadNum())
	{
		controller_ = std::make_unique<PadInput>();
	}
	else
	{
		controller_ = std::make_unique<KeyInput>();
	}
	pos_.x = lpSceneMng.GetScreenSize().x / 2.0f;
	pos_.y = lpSceneMng.GetScreenSize().y / 2.0f;
}

void MouseIcon::Release(void)
{
}

void MouseIcon::Update(SceneID id)
{
	sceneID_ = id;
}

void MouseIcon::Draw(void)
{
	if (GetJoypadNum())
	{
		//PAD接続中
		DINPUT_JOYSTATE rStick_Input;
		//右スティックの入力状態を取得
		GetJoypadDirectInputState(DX_INPUT_PAD1, &rStick_Input);
		pos_.x += rStick_Input.Rx / DIV_SENSITIVITY;
		pos_.y += rStick_Input.Ry / DIV_SENSITIVITY;
		if (pos_.x < GRAPH_SIZE)
		{
			pos_.x = GRAPH_SIZE;
		}
		if (pos_.y < GRAPH_SIZE)
		{
			pos_.y = GRAPH_SIZE;
		}
		if (pos_.x > lpSceneMng.GetScreenSize().x - GRAPH_SIZE)
		{
			pos_.x = lpSceneMng.GetScreenSize().x - GRAPH_SIZE;
		}
		if (pos_.y > lpSceneMng.GetScreenSize().y - GRAPH_SIZE)
		{
			pos_.y = lpSceneMng.GetScreenSize().y - GRAPH_SIZE;
		}
		switch (sceneID_)
		{
		case SceneID::Inventory:
		case SceneID::GameOver:
		case SceneID::Title:
		case SceneID::Option:
		case SceneID::Game:
		case SceneID::Select:
		case SceneID::Result:
		case SceneID::Load:
		case SceneID::Exit:
		case SceneID::Max:
			SetMousePoint(static_cast<int>(pos_.x), static_cast<int>(pos_.y));
			break;
		default:
			break;
		}
	}
	else
	{
		int mx, my;
		//PAD未接続
		//マウスの位置更新
		GetMousePoint(&mx, &my);

		pos_.x = static_cast<float>(mx);
		pos_.y = static_cast<float>(my);
	}

	if (sceneID_ == SceneID::Game)
	{
		if (flg_)
		{
			//パッド接続しているか
			if (GetJoypadNum() > 0)
			{
				DrawRotaGraph(lpSceneMng.GetScreenSize().x / 2, (lpSceneMng.GetScreenSize().y / 2) + 40, 1.0f, 0.0f, lpImageMng.GetID("./resource/image/mouse/interactPadText.png")[0], true);
			}
			else
			{
				DrawRotaGraph(lpSceneMng.GetScreenSize().x / 2, (lpSceneMng.GetScreenSize().y / 2) + 40, 1.0f, 0.0f, lpImageMng.GetID("./resource/image/mouse/interactText.png")[0], true);
			}
			//インタラクトできるときマウスアイコン変更
			DrawRotaGraph(lpSceneMng.GetScreenSize().x / 2, lpSceneMng.GetScreenSize().y / 2, 1.0f, 0.0f, lpImageMng.GetID("./resource/image/mouse/Getreticle.png")[0], true);
			
		}
		else
		{
			DrawRotaGraph(lpSceneMng.GetScreenSize().x / 2, lpSceneMng.GetScreenSize().y / 2, 1.0f, 0.0f, lpImageMng.GetID("./resource/image/mouse/reticle.png")[0], true);
		}
		//DrawRotaGraph(lpSceneMng.GetScreenSize().x / 2, lpSceneMng.GetScreenSize().y / 2, 1.0f, 0.0f, lpImageMng.GetID("./resource/image/mouse/black.png")[0], true);
	}
	else if(sceneID_ == SceneID::GameOver)
	{
		if (flg_)
		{
			DrawRotaGraph(static_cast<int>(pos_.x) + MOUSEICON_OFFSET_X, static_cast<int>(pos_.y) + MOUSEICON_OFFSET_Y, 1.0f, 0.0f, lpImageMng.GetID("./resource/image/mouse/mouse.png")[0], true);
		}
		else
		{

		}
	}
	else
	{
		DrawRotaGraph(static_cast<int>(pos_.x) + MOUSEICON_OFFSET_X, static_cast<int>(pos_.y) + MOUSEICON_OFFSET_Y, 1.0f, 0.0f, lpImageMng.GetID("./resource/image/mouse/mouse.png")[0], true);
	}
}


