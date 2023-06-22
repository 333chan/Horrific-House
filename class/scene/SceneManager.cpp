#include <DxLib.h>
#include"EffekseerForDXLib.h"
#include "PreTitleScene.h"
#include"TitleScene.h"
#include"GameScene.h"
#include"LoadScene.h"
#include "InventoryScene.h"
#include "BeginningScene.h"
#include "../Transition/FadeinOut.h"
#include "SceneManager.h"
#include "../common/SocketServer.h"
#include"../common/MouseIcon.h"
#include "../Common/Fps.h"
#include "../../resource/image/icon/icon.h"
#include "../../_debug/_DebugDispOut.h"
//��ʃT�C�Y
constexpr int SCREEN_SIZE_X = 1920;	//��
constexpr int SCREEN_SIZE_Y = 1080;	//�c

SceneManager::SceneManager()
{
	
}

SceneManager::~SceneManager()
{
	lpSocketServer.CommunicationCutoff();
}


void SceneManager::Run(void)
{
	if (!SysInit())
	{
		return;
	}
	//������
	Init();
	//�Q�[�����[�v
	while (ProcessMessage() == 0)
	{
		//�f�o�b�O�J�n
		_dbgStartDraw();
		//�X�V
		Update();

		Draw();

		ScreenFlip();
		fps_->Wait();
	}
	//��ɃE�B���h�E�����
	//SetWindowVisibleFlag(false);
	//effkseer�I��
	Effkseer_End();
	//dxlib�I��
	DxLib_End();
}

bool SceneManager::SysInit(void)
{
	SetGraphMode(static_cast<int>(SCREEN_SIZE_X), static_cast<int>(SCREEN_SIZE_Y), 32);
	ChangeWindowMode(false);
	//ChangeWindowMode(false);

	SetEnableXAudioFlag(true);
	// 64.0f �łP���[�g���Ƃ���
	Set3DSoundOneMetre(1500.0f);
	SetWindowIconID(ICON_1);
	SetWindowText("Horrific House");
	if (DxLib_Init() == -1)
	{
		return false;
	}
#ifdef _DEBUG
	SetMouseDispFlag(true);
#else
	SetMouseDispFlag(false);
#endif
	_dbgSetup(static_cast<int>(SCREEN_SIZE_X), static_cast<int>(SCREEN_SIZE_Y), 255);
	return true;
}

void SceneManager::Init(void)
{
	//�V�[��������
	scene_ = std::make_unique<FadeinOut>(1.0f,Transition::In,std::make_unique<BeginningScene>());
	//
	sceneGraphFilter_ = MakeScreen(SCREEN_SIZE_X, SCREEN_SIZE_Y);
	brightnessScrren_ = 255;
	//�f���^�^�C��������
	deltaTime_ = 0.0f;
	tickCount_ = std::chrono::system_clock::now();
	brightnessScrren_ = 255;
	//mouseIcon_ = std::make_unique<MouseIcon>();
	Init3D();
	//--Effekseer������--
	Effekseer_Init(8000);
	SetChangeScreenModeGraphicsSystemResetFlag(false);
	Effekseer_SetGraphicsDeviceLostCallbackFunctions();
	fps_ = std::make_unique<Fps>();
	//-------------------
	lpSocketServer.InternetConnection();
}

void SceneManager::Init3D(void)
{
	// Z�o�b�t�@��L���ɂ���
	SetUseZBuffer3D(true);

	// Z�o�b�t�@�ւ̏������݂�L���ɂ���
	SetWriteZBuffer3D(true);

	// �o�b�N�J�����O��L���ɂ���
	SetUseBackCulling(true);

	// �N���b�v������ݒ肷��
	//SetCameraNearFar(CAMERA_RANGE_NEAR, CAMERA_RANGE_FAR);

	//// ���C�g�̐ݒ�
	//ChangeLightTypeDir({ 0.3f, -0.7f, 0.8f });

	// �w�i�F�ݒ�
	SetBackgroundColor(0, 0, 0);
	
}

void SceneManager::Update(void)
{
	lpSocketServer.Receive();
	UpdateMouse();

	auto tick = std::chrono::system_clock::now();
	deltaTime_ = std::chrono::duration_cast<std::chrono::microseconds>(tick - tickCount_).count() / 1000000.0f;
	tickCount_ = tick;

	fps_->Update();

	//�X�V
	scene_ = scene_->Update(std::move(scene_));
	lpMouseIcon.Update(scene_->GetSceneID());
}

void SceneManager::UpdateMouse()
{
	Vector2Int Mpos;
	GetMousePoint(&Mpos.x, &Mpos.y);
	if (scene_->GetSceneID() == SceneID::Game)
	{
		Mpos.x += lpSocketServer.GetCameraValue().first;
		Mpos.y -= lpSocketServer.GetCameraValue().second;
	}
	else
	{
		Mpos.x += lpSocketServer.GetCameraValue().first / 10;
		Mpos.y -= lpSocketServer.GetCameraValue().second / 10;
	}
	SetMousePoint(Mpos.x, Mpos.y);
}

void SceneManager::Draw(void)
{
	SetDrawScreen(sceneGraphFilter_);
	ClsDrawScreen();
	scene_->Draw();

	SetDrawScreen(DX_SCREEN_BACK);
	ClsDrawScreen();
	DrawGraph(0, 0, sceneGraphFilter_, true);

	lpMouseIcon.Draw();

	_dbgDraw();

}

Vector2Int SceneManager::GetScreenSize(void)
{
	return Vector2Int(SCREEN_SIZE_X, SCREEN_SIZE_Y);
}

double SceneManager::GetDeltaTime(void)
{
	return deltaTime_;
}

void SceneManager::SetBrightnessScrren(int brightnessScrren)
{
	brightnessScrren_ = brightnessScrren;
}

int SceneManager::GetBrightnessScrren()
{
	return brightnessScrren_;
}

void SceneManager::AddInventoryCnt()
{
	inventoryCnt_++;
}

int SceneManager::GetInventoryCnt()
{
	return inventoryCnt_;
}
