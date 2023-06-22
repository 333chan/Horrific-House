#include<DxLib.h>
#include "MouseIcon.h"
#include"../common/manager/ImageMng.h"
#include"../scene/SceneManager.h"
#include"../Input/KeyInput.h"
#include"../Input/PadInput.h"

constexpr int LOWER_LIMIT = 200;		//�X���̍Œᐔ�l�͂Q�O�O

//���x
constexpr float DIV_SENSITIVITY = 40.0f;

//�摜�T�C�Y
constexpr float GRAPH_SIZE = 16.0f;

//�I�t�Z�b�g
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
		//PAD�ڑ���
		DINPUT_JOYSTATE rStick_Input;
		//�E�X�e�B�b�N�̓��͏�Ԃ��擾
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
		//PAD���ڑ�
		//�}�E�X�̈ʒu�X�V
		GetMousePoint(&mx, &my);

		pos_.x = static_cast<float>(mx);
		pos_.y = static_cast<float>(my);
	}

	if (sceneID_ == SceneID::Game)
	{
		if (flg_)
		{
			//�p�b�h�ڑ����Ă��邩
			if (GetJoypadNum() > 0)
			{
				DrawRotaGraph(lpSceneMng.GetScreenSize().x / 2, (lpSceneMng.GetScreenSize().y / 2) + 40, 1.0f, 0.0f, lpImageMng.GetID("./resource/image/mouse/interactPadText.png")[0], true);
			}
			else
			{
				DrawRotaGraph(lpSceneMng.GetScreenSize().x / 2, (lpSceneMng.GetScreenSize().y / 2) + 40, 1.0f, 0.0f, lpImageMng.GetID("./resource/image/mouse/interactText.png")[0], true);
			}
			//�C���^���N�g�ł���Ƃ��}�E�X�A�C�R���ύX
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


