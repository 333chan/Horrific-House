#include "MinCamera.h"
#include"../../common/OriginalShader.h"
#include "../../common/manager/ImageMng.h"
#include "../../common/manager/ModelMng.h"

#include "../../scene/SceneManager.h"
#include "../../common/manager/CBufferMng.h"
#include"../../input/Controller.h"
#include "../../input/PadInput.h"
#include "../../input/KeyInput.h"
#include "MinCameraUI.h"
#include "../../common/manager/TutorialMng.h"

//スマホの大きさ
constexpr float SIZE_SCALE = 20.0f;

//初期値
constexpr float INIT_POS_X = 1100.0f;
constexpr float INIT_POS_Y = -600.0f;
constexpr float INIT_POS_Z = 200.0f;
constexpr float INIT_ANGLE = 1.0f;
//移動速度
constexpr float MOVE_POWER_X = 30.0f;
constexpr float MOVE_POWER_Y = 100.0f;
constexpr float ROTATION_POWER = 0.1f;

//フラッシュ
constexpr double WHITE_LIMIT_TIME = 0.2f;//画面全体が白い時間
constexpr float SUB_WHITE_TIME = 0.015f;

//時間カウント
constexpr float TIME_COUNT = 0.02f;

//メモ関係
constexpr int    MEMO_PHOTO_NUM    = 3;		 //写真の数

//硬直用定数
constexpr double MEMO_STOP_TIME  = 0.35;	 //硬直時間

//画面縮小用定数
constexpr double  MEMO_LERP_TIME = 1.0f;	 //移動補間時間
constexpr float  MEMO_MOVED_POS_X  = 100.0;	 //移動後の位置ｘ
constexpr float  MEMO_MOVED_POS_Y  = 730.0f; //移動後の位置ｙ
constexpr float  MEMO_MOVED_SIZE_X = 520.0f; //移動後のサイズｘ
constexpr float  MEMO_MOVED_SIZE_Y = 250.0f; //移動後のサイズｙ

//下移動用定数
constexpr double MEMO_DOWN_TIME = 2.0;     //落下時間
constexpr double MEMO_INIT_VELOCITY = 20.0;	 //鉛直投げ上げの初速度
constexpr double MEMO_ADD_ACCEL = 2.0;	 //鉛直投げ上げの初速度

constexpr int    INPUT_TIGGER_MAX = 255;		 //RTの反応入力値

MinCamera::MinCamera(ControllerType type)
{
	if (type == ControllerType::Pad)
	{
		controller_ = std::make_unique<PadInput>();
	}
	else
	{
		controller_ = std::make_unique<KeyInput>();
	}
	//初期化
	Init();
}

MinCamera::~MinCamera()
{
	DeleteGraph(renderTarget_);
	DeleteShader(psCameraHandle_);
	DeleteShader(psMemoHandle_);
	for (auto photo : photoVector_)
	{
		DeleteGraph(photo);
	}
}

void MinCamera::Init(void)
{
	//メモ写真情報
	photoVector_.resize(MEMO_PHOTO_NUM);
	photoCount_ = 0;
	stepMemoTime_ = 0.0;
	stepMemoLerpTime_ = MEMO_LERP_TIME;
	moveState_ = MoveState::NONE;
	oldMoveState_ = moveState_;

	//頂点情報初期化
	InitMemoPosition();

	for (auto& photo : photoVector_)
	{
		photo = MakeGraph(lpSceneMng.GetScreenSize().x, lpSceneMng.GetScreenSize().y);
	}
	
	//モデル関係初期化
	pos_ = { INIT_POS_X,INIT_POS_Y,INIT_POS_Z };
	MV1SetScale(lpModelMng.GetID("resource/model/smh.mv1")[0], VGet(SIZE_SCALE, SIZE_SCALE, SIZE_SCALE));
	MV1SetPosition(lpModelMng.GetID("resource/model/smh.mv1")[0], pos_.toVECTOR());

	//シェーダ関係初期化
	origShader_ = std::make_unique<OriginalShader>(LoadVertexShader("resource/shader/vs/MinCamera_vs.vso"), LoadPixelShader("resource/shader/ps/MinCamera_ps.pso"));
	onCamera_ = false;
	psCameraHandle_ = LoadPixelShader("./resource/shader/ps/Camera_ps.pso");
	psMemoHandle_ = LoadPixelShader("./resource/shader/ps/Memo_ps.pso");
	cameraScreen_ = std::make_unique<OriginalShader>(psCameraHandle_);
	memoScreen_ = std::make_unique<OriginalShader>(psMemoHandle_);

	//UI
	minCameraUI_ = std::make_unique<MinCameraUI>(controller_->GetControllerType());

	//レンダーターゲット
	renderTarget_ = MakeScreen(lpSceneMng.GetScreenSize().x, lpSceneMng.GetScreenSize().y);
	stepWhiteTime_ = 0.0;
	flashOneFrameFlag_ = false;

	//定数バッファ
	whiteRange_ = -1.0f;
	time_ = 0.0f;
	lpCBufferMng.SetPixelCbuffer(8, 0, whiteRange_);
	lpCBufferMng.SetPixelCbuffer(8, 1, time_);
}

void MinCamera::InitMemoPosition(void)
{
	Vector2 screenSize = {static_cast<float>(lpSceneMng.GetScreenSize().x),static_cast<float>(lpSceneMng.GetScreenSize().y) };
	memoPos_[0] = Vector3();
	memoPos_[1] = Vector3(screenSize.x, 0.0f, 0.0f);
	memoPos_[2] = Vector3(0.0f, screenSize.y, 0.0f);
	memoPos_[3] = Vector3(screenSize.x, screenSize.y, 0.0f);
}

void MinCamera::Update(bool IsCamera)
{
	controller_->Update();

	flashOneFrameFlag_ = false;
	oldMoveState_ = moveState_;

	
	if (controller_->CheckInputKey(KeyID::Cancel))
	{
		//初めてカメラ構えた時、Tutorialflgをtrue
		lpTutorialMng.SetTutorialFlg(TutorialProgress::SetupCamera, true);
		onCamera_ = !onCamera_;
	}

	//カメラの位置更新
	MoveCamera();

	//フラッシュ処理
	FlashCamera();

	//メモ処理
	MemoProcess();

	//定数バッファ更新
	lpCBufferMng.SetPixelCbuffer(8, 0, whiteRange_);
	lpCBufferMng.SetPixelCbuffer(8, 1, time_);
	lpCBufferMng.SetVertexCbuffer(5, 0, time_);

	time_ += TIME_COUNT;
}

void MinCamera::Draw(void)
{
	//カメラを構えているかどうか
	if (onCamera_)
	{
		MV1SetTextureGraphHandle(lpModelMng.GetID("resource/model/smh.mv1")[0], 1, renderTarget_, false);
		MV1DrawModel(lpModelMng.GetID("resource/model/smh.mv1")[0]);
		minCameraUI_->Draw();
	}
}

bool MinCamera::GetIsOnCamera()const
{
	return onCamera_;
}

void MinCamera::SetIsOnCamera(bool IsCamera)
{
	onCamera_ = IsCamera;
}

void MinCamera::MoveSub(float& pos, float max, float value)
{
	pos -= value;

	if (pos < max)
	{
		pos = max;
	}
}

void MinCamera::MoveAdd(float& pos, float max, float value)
{
	pos += value;

	if (pos > max)
	{
		pos = max;
	}
}

bool MinCamera::GetFlashFlag(void)
{
	return flashOneFrameFlag_;
}

void MinCamera::SetRenderTarget(int screenID)
{
	//現在スクリーン描画されているものを保存
	if (moveState_ == MoveState::STOP && moveState_ != oldMoveState_)
	{
		GetDrawScreenGraph(0, 0, lpSceneMng.GetScreenSize().x, lpSceneMng.GetScreenSize().y, photoVector_[photoCount_ % MEMO_PHOTO_NUM]);
		photoCount_++;
	}

	SetDrawScreen(renderTarget_);
	ClsDrawScreen();
	cameraScreen_->DrawOnlyPixel(0.0f, 0.0f, screenID, lpImageMng.GetID("./resource/shader/ps/texture/noise7.png")[0], -1, -1);
	//メモ
	if (moveState_ != MoveState::NONE)
	{
		memoScreen_->DrawOnlyPixel(memoPos_, photoVector_[(photoCount_ - 1) % MEMO_PHOTO_NUM]);
	}
}

std::vector<int>& MinCamera::GetPhotoVector(void)
{
	return photoVector_;
}

void MinCamera::MoveCamera(void)
{
	XINPUT_STATE state;
	GetJoypadXInputState(DX_INPUT_PAD1, &state);
	auto cntType = controller_->GetControllerType();
	if (onCamera_)
	{
		//回転
		MoveSub(angle_.y, 0.0f, ROTATION_POWER);
		//X軸移動
		MoveSub(pos_.x, lpSceneMng.GetScreenSize().x / 2.0f, MOVE_POWER_X);
		//Y軸移動
		MoveAdd(pos_.y, lpSceneMng.GetScreenSize().y / 2.0f, MOVE_POWER_Y);
		if ((cntType== ControllerType::Key && controller_->CheckInputKey(KeyID::Decide) && whiteRange_ <= 0.0f) ||
			(cntType == ControllerType::Pad && state.LeftTrigger == INPUT_TIGGER_MAX && whiteRange_ <= 0.0f))
		{
			whiteRange_ = 1.0f;
			stepWhiteTime_ = 0.0f;
			flashOneFrameFlag_ = true;
			//初回攻撃時、Tutorialflgをtrue
			lpTutorialMng.SetTutorialFlg(TutorialProgress::Flash, true);
		}
	}
	else
	{
		//アンドロイド用処理
		if (controller_->CheckInputKey(KeyID::Attack) && whiteRange_ <= 0.0f)
		{
			whiteRange_ = 1.0f;
			stepWhiteTime_ = 0.0f;
			flashOneFrameFlag_ = true;
			//初回攻撃時、Tutorialflgをtrue
			lpTutorialMng.SetTutorialFlg(TutorialProgress::Flash, true);
		}
		angle_.y = INIT_ANGLE;
		pos_.x = INIT_POS_X;
		pos_.y = INIT_POS_Y;
	}

	//位置適用
	MV1SetPosition(lpModelMng.GetID("resource/model/smh.mv1")[0], pos_.toVECTOR());
	MV1SetRotationXYZ(lpModelMng.GetID("resource/model/smh.mv1")[0], angle_.toVECTOR());
}

void MinCamera::FlashCamera(void)
{
	if (WHITE_LIMIT_TIME < stepWhiteTime_)
	{
		//制限時間を超えた場合は徐々に視界を広げる
		if (whiteRange_ > 0.0f)
		{
			whiteRange_ -= SUB_WHITE_TIME;
		}
	}
	stepWhiteTime_ += lpSceneMng.GetDeltaTime();
}

void MinCamera::MemoProcess(void)
{
	bool hitKeyFlag = false;
	XINPUT_STATE state;
	GetJoypadXInputState(DX_INPUT_PAD1, &state);
	auto cntType = controller_->GetControllerType();

	switch (moveState_)
	{
	case MoveState::NONE:
		//いつでも保存できるため、スイッチの外に置く
		if ((cntType == ControllerType::Pad && state.RightTrigger == INPUT_TIGGER_MAX && onCamera_) ||
			(cntType == ControllerType::Key && controller_->CheckInputKey(KeyID::Shoot) && onCamera_))
		{
			hitKeyFlag = true;
			//初回撮影時、Tutorialflgをtrue
			lpTutorialMng.SetTutorialFlg(TutorialProgress::Snap, true);
		}
		else if(controller_->CheckInputKey(KeyID::ShootForAndroid)/* && onCamera_*/)
		{
			hitKeyFlag = true;
			//初回撮影時、Tutorialflgをtrue
			lpTutorialMng.SetTutorialFlg(TutorialProgress::Snap, true);
		}
		//キーが押されていた場合
		if (hitKeyFlag)
		{
			moveState_ = MoveState::STOP;
			stepMemoTime_ = 0.0;
			InitMemoPosition();
		}
		break;
	case MoveState::STOP:
		if (stepMemoTime_ > MEMO_STOP_TIME)
		{
			moveState_ = MoveState::SHRINK;
			stepMemoLerpTime_ = MEMO_LERP_TIME;
		}
		break;
	case MoveState::SHRINK:
	{
		//補間時間計算
		float lerpTime = 1.0f - (stepMemoLerpTime_ / MEMO_LERP_TIME);
		//補間
		memoPos_[0] = Lerp(memoPos_[0], Vector3(MEMO_MOVED_POS_X, MEMO_MOVED_POS_Y, 0.0f), lerpTime);
		memoPos_[1] = Lerp(memoPos_[1], Vector3(MEMO_MOVED_POS_X + MEMO_MOVED_SIZE_X, MEMO_MOVED_POS_Y, 0.0f), lerpTime);
		memoPos_[2] = Lerp(memoPos_[2], Vector3(MEMO_MOVED_POS_X, MEMO_MOVED_POS_Y + MEMO_MOVED_SIZE_Y, 0.0f), lerpTime);
		memoPos_[3] = Lerp(memoPos_[3], Vector3(MEMO_MOVED_POS_X + MEMO_MOVED_SIZE_X, MEMO_MOVED_POS_Y + MEMO_MOVED_SIZE_Y, 0.0f), lerpTime);
		stepMemoLerpTime_ -= lpSceneMng.GetDeltaTime();

		if (lerpTime > 1.0)
		{
			stepMemoTime_ = 0.0;
			downAccel_ = 0.0;
			moveState_ = MoveState::DOWN;
		}
		break;
	}
	case MoveState::DOWN:
	{
		// 物体の位置 = 初速度 * 時間 + 1 / 2( 加速度 * 時間の二乗 ) 
		//y(pos.y) = vo(初速度)t(時間)－1 / 2 * g(accel) * t^2

		//落下速度(最初は上に上昇し、時間経過で落下)
		float downSpeed = static_cast<float>(
			(-MEMO_INIT_VELOCITY * stepMemoTime_) +
			((1.0 / 2.0) * (downAccel_ * pow(stepMemoTime_, 2.0))));

		for (int i = 0; i < MEMO_VERTS_NUM; i++)
		{
			memoPos_[i].y += downSpeed;
		}

		//落下加速度加算
		downAccel_ += MEMO_ADD_ACCEL;

		if (stepMemoTime_ > MEMO_DOWN_TIME)
		{
			moveState_ = MoveState::NONE;
		}
		break;
	}
	default:
		break;
	}

	//時間経過
	stepMemoTime_ += lpSceneMng.GetDeltaTime();
}
