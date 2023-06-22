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

//�X�}�z�̑傫��
constexpr float SIZE_SCALE = 20.0f;

//�����l
constexpr float INIT_POS_X = 1100.0f;
constexpr float INIT_POS_Y = -600.0f;
constexpr float INIT_POS_Z = 200.0f;
constexpr float INIT_ANGLE = 1.0f;
//�ړ����x
constexpr float MOVE_POWER_X = 30.0f;
constexpr float MOVE_POWER_Y = 100.0f;
constexpr float ROTATION_POWER = 0.1f;

//�t���b�V��
constexpr double WHITE_LIMIT_TIME = 0.2f;//��ʑS�̂���������
constexpr float SUB_WHITE_TIME = 0.015f;

//���ԃJ�E���g
constexpr float TIME_COUNT = 0.02f;

//�����֌W
constexpr int    MEMO_PHOTO_NUM    = 3;		 //�ʐ^�̐�

//�d���p�萔
constexpr double MEMO_STOP_TIME  = 0.35;	 //�d������

//��ʏk���p�萔
constexpr double  MEMO_LERP_TIME = 1.0f;	 //�ړ���Ԏ���
constexpr float  MEMO_MOVED_POS_X  = 100.0;	 //�ړ���̈ʒu��
constexpr float  MEMO_MOVED_POS_Y  = 730.0f; //�ړ���̈ʒu��
constexpr float  MEMO_MOVED_SIZE_X = 520.0f; //�ړ���̃T�C�Y��
constexpr float  MEMO_MOVED_SIZE_Y = 250.0f; //�ړ���̃T�C�Y��

//���ړ��p�萔
constexpr double MEMO_DOWN_TIME = 2.0;     //��������
constexpr double MEMO_INIT_VELOCITY = 20.0;	 //���������グ�̏����x
constexpr double MEMO_ADD_ACCEL = 2.0;	 //���������グ�̏����x

constexpr int    INPUT_TIGGER_MAX = 255;		 //RT�̔������͒l

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
	//������
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
	//�����ʐ^���
	photoVector_.resize(MEMO_PHOTO_NUM);
	photoCount_ = 0;
	stepMemoTime_ = 0.0;
	stepMemoLerpTime_ = MEMO_LERP_TIME;
	moveState_ = MoveState::NONE;
	oldMoveState_ = moveState_;

	//���_��񏉊���
	InitMemoPosition();

	for (auto& photo : photoVector_)
	{
		photo = MakeGraph(lpSceneMng.GetScreenSize().x, lpSceneMng.GetScreenSize().y);
	}
	
	//���f���֌W������
	pos_ = { INIT_POS_X,INIT_POS_Y,INIT_POS_Z };
	MV1SetScale(lpModelMng.GetID("resource/model/smh.mv1")[0], VGet(SIZE_SCALE, SIZE_SCALE, SIZE_SCALE));
	MV1SetPosition(lpModelMng.GetID("resource/model/smh.mv1")[0], pos_.toVECTOR());

	//�V�F�[�_�֌W������
	origShader_ = std::make_unique<OriginalShader>(LoadVertexShader("resource/shader/vs/MinCamera_vs.vso"), LoadPixelShader("resource/shader/ps/MinCamera_ps.pso"));
	onCamera_ = false;
	psCameraHandle_ = LoadPixelShader("./resource/shader/ps/Camera_ps.pso");
	psMemoHandle_ = LoadPixelShader("./resource/shader/ps/Memo_ps.pso");
	cameraScreen_ = std::make_unique<OriginalShader>(psCameraHandle_);
	memoScreen_ = std::make_unique<OriginalShader>(psMemoHandle_);

	//UI
	minCameraUI_ = std::make_unique<MinCameraUI>(controller_->GetControllerType());

	//�����_�[�^�[�Q�b�g
	renderTarget_ = MakeScreen(lpSceneMng.GetScreenSize().x, lpSceneMng.GetScreenSize().y);
	stepWhiteTime_ = 0.0;
	flashOneFrameFlag_ = false;

	//�萔�o�b�t�@
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
		//���߂ăJ�����\�������ATutorialflg��true
		lpTutorialMng.SetTutorialFlg(TutorialProgress::SetupCamera, true);
		onCamera_ = !onCamera_;
	}

	//�J�����̈ʒu�X�V
	MoveCamera();

	//�t���b�V������
	FlashCamera();

	//��������
	MemoProcess();

	//�萔�o�b�t�@�X�V
	lpCBufferMng.SetPixelCbuffer(8, 0, whiteRange_);
	lpCBufferMng.SetPixelCbuffer(8, 1, time_);
	lpCBufferMng.SetVertexCbuffer(5, 0, time_);

	time_ += TIME_COUNT;
}

void MinCamera::Draw(void)
{
	//�J�������\���Ă��邩�ǂ���
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
	//���݃X�N���[���`�悳��Ă�����̂�ۑ�
	if (moveState_ == MoveState::STOP && moveState_ != oldMoveState_)
	{
		GetDrawScreenGraph(0, 0, lpSceneMng.GetScreenSize().x, lpSceneMng.GetScreenSize().y, photoVector_[photoCount_ % MEMO_PHOTO_NUM]);
		photoCount_++;
	}

	SetDrawScreen(renderTarget_);
	ClsDrawScreen();
	cameraScreen_->DrawOnlyPixel(0.0f, 0.0f, screenID, lpImageMng.GetID("./resource/shader/ps/texture/noise7.png")[0], -1, -1);
	//����
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
		//��]
		MoveSub(angle_.y, 0.0f, ROTATION_POWER);
		//X���ړ�
		MoveSub(pos_.x, lpSceneMng.GetScreenSize().x / 2.0f, MOVE_POWER_X);
		//Y���ړ�
		MoveAdd(pos_.y, lpSceneMng.GetScreenSize().y / 2.0f, MOVE_POWER_Y);
		if ((cntType== ControllerType::Key && controller_->CheckInputKey(KeyID::Decide) && whiteRange_ <= 0.0f) ||
			(cntType == ControllerType::Pad && state.LeftTrigger == INPUT_TIGGER_MAX && whiteRange_ <= 0.0f))
		{
			whiteRange_ = 1.0f;
			stepWhiteTime_ = 0.0f;
			flashOneFrameFlag_ = true;
			//����U�����ATutorialflg��true
			lpTutorialMng.SetTutorialFlg(TutorialProgress::Flash, true);
		}
	}
	else
	{
		//�A���h���C�h�p����
		if (controller_->CheckInputKey(KeyID::Attack) && whiteRange_ <= 0.0f)
		{
			whiteRange_ = 1.0f;
			stepWhiteTime_ = 0.0f;
			flashOneFrameFlag_ = true;
			//����U�����ATutorialflg��true
			lpTutorialMng.SetTutorialFlg(TutorialProgress::Flash, true);
		}
		angle_.y = INIT_ANGLE;
		pos_.x = INIT_POS_X;
		pos_.y = INIT_POS_Y;
	}

	//�ʒu�K�p
	MV1SetPosition(lpModelMng.GetID("resource/model/smh.mv1")[0], pos_.toVECTOR());
	MV1SetRotationXYZ(lpModelMng.GetID("resource/model/smh.mv1")[0], angle_.toVECTOR());
}

void MinCamera::FlashCamera(void)
{
	if (WHITE_LIMIT_TIME < stepWhiteTime_)
	{
		//�������Ԃ𒴂����ꍇ�͏��X�Ɏ��E���L����
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
		//���ł��ۑ��ł��邽�߁A�X�C�b�`�̊O�ɒu��
		if ((cntType == ControllerType::Pad && state.RightTrigger == INPUT_TIGGER_MAX && onCamera_) ||
			(cntType == ControllerType::Key && controller_->CheckInputKey(KeyID::Shoot) && onCamera_))
		{
			hitKeyFlag = true;
			//����B�e���ATutorialflg��true
			lpTutorialMng.SetTutorialFlg(TutorialProgress::Snap, true);
		}
		else if(controller_->CheckInputKey(KeyID::ShootForAndroid)/* && onCamera_*/)
		{
			hitKeyFlag = true;
			//����B�e���ATutorialflg��true
			lpTutorialMng.SetTutorialFlg(TutorialProgress::Snap, true);
		}
		//�L�[��������Ă����ꍇ
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
		//��Ԏ��Ԍv�Z
		float lerpTime = 1.0f - (stepMemoLerpTime_ / MEMO_LERP_TIME);
		//���
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
		// ���̂̈ʒu = �����x * ���� + 1 / 2( �����x * ���Ԃ̓�� ) 
		//y(pos.y) = vo(�����x)t(����)�|1 / 2 * g(accel) * t^2

		//�������x(�ŏ��͏�ɏ㏸���A���Ԍo�߂ŗ���)
		float downSpeed = static_cast<float>(
			(-MEMO_INIT_VELOCITY * stepMemoTime_) +
			((1.0 / 2.0) * (downAccel_ * pow(stepMemoTime_, 2.0))));

		for (int i = 0; i < MEMO_VERTS_NUM; i++)
		{
			memoPos_[i].y += downSpeed;
		}

		//���������x���Z
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

	//���Ԍo��
	stepMemoTime_ += lpSceneMng.GetDeltaTime();
}
