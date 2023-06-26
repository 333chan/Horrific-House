#include<DxLib.h>
#include<EffekseerForDXLib.h>
#include"../Scene/SceneManager.h"
#include "Camera.h"
#include "../common/manager/SoundMng.h"
#include "../common/manager/TutorialMng.h"

//�����_����J�����܂ł̋���
constexpr float TARGET2CAMERA_DISTANCE = 500.0f;

//X����]���̐����p�x
constexpr float UP_RADIAN_LIMIT_X  = -70.0f * (DX_PI_F / 180.0f);	//�����
constexpr float DOWN_RADIAN_LIMIT_X = 60.0f * (DX_PI_F / 180.0f);	//������ 60

//�J�����̕`�拗��
constexpr float CAMERA_RANGE_NEAR = 50.0f;		//��O
constexpr float CAMERA_RANGE_FAR  = 12500.0f;	//��

//��ʗh��
constexpr float SWING_WIDTH = 0.125f * (DX_PI_F / 180.0f);
constexpr float SWING_OFFSET = 3.5f;

//���S���o���̃J�����̐���
constexpr float LIFT_HEIGHT = 800.0f;	//�G���v���C���[�������グ�鎞�̍���
constexpr float LIFT_SPEED = 8.0f;	//�㏸���x
constexpr float ZERO = 0.0f;	//0
constexpr float ROLL_LIMIT = 6.0f;	//��]�̌��E�l
constexpr float ROLL_SPEED = 60.0f * (DX_PI_F / 180.0f);	//��]���x

Camera::Camera()
{
	Init();
}
Camera::Camera(Vector3 cameraAngle)
{
	Init();
	angle_ = cameraAngle;
	defaultangle_y = angle_.y;
}

Camera::~Camera()
{
}

void Camera::Init(void)
{
	//�ʒu
	pos_ = { 0.0f,CAMERA_HEIGHT,-TARGET2CAMERA_DISTANCE };
	offset_ = { 0.0f,0.0f,0.0f };
	t_offset_ = { 0.0f,1400.0f,0.0f };
	//�p�x
	angle_ = {
		0.0f,
		0.0f,
		0.0f
	};
	quaRot_ = Quaternion();
	roll_ = 0.0f;
	sinCnt_ = 0;
	fallCnt_ = 0.0;
	liftFlg_ = true;
	waitTime_ = 0.0;
	sinCount_ = 0.0f;
	sensitivity_ = 15.0f;
	// �N���b�v�����̐ݒ�
	SetCameraNearFar(CAMERA_RANGE_NEAR, CAMERA_RANGE_FAR);

	//�}�E�X�̈ʒu��^�񒆂ɌŒ�
	SetMousePoint(lpSceneMng.GetScreenSize().x / 2, lpSceneMng.GetScreenSize().y / 2);
}

void Camera::Update(bool dashFlag)
{
	if (GetJoypadNum())
	{
		//PAD�ڑ���
		DINPUT_JOYSTATE rStick_Input;
		//�E�X�e�B�b�N�̓��͏�Ԃ��擾
		GetJoypadDirectInputState(DX_INPUT_PAD1, &rStick_Input);

		Vector2Int middlePadPos = { lpSceneMng.GetScreenSize().x / 2, lpSceneMng.GetScreenSize().y / 2 };
		
		padPos_.x += rStick_Input.Rx / sensitivity_;
		padPos_.y += rStick_Input.Ry / sensitivity_;

		//�}�E�X�̈ړ��������v�Z
		Vector2 padMoveDistance = { static_cast<float>(padPos_.x - middlePadPos.x),static_cast<float>(padPos_.y - middlePadPos.y) };

		//�}�E�X�ʒu�̍����ŉ�]������
		angle_.y += Deg2RadF(padMoveDistance.x / 30.0f);	//���E��]
		angle_.x += Deg2RadF(padMoveDistance.y / 30.0f);	//�㉺��]
		//������̊p�x����
		if (angle_.x < UP_RADIAN_LIMIT_X)
		{
			angle_.x = UP_RADIAN_LIMIT_X;
		}

		//�������̊p�x����
		if (angle_.x > DOWN_RADIAN_LIMIT_X)
		{
			angle_.x = DOWN_RADIAN_LIMIT_X;
		}

		if (dashFlag)
		{
			angle_.x += SwingWidth();
		}

		padPos_.x = middlePadPos.x;
		padPos_.y = middlePadPos.y;
	}
	else
	{
		Vector2Int middleMousePos = { lpSceneMng.GetScreenSize().x / 2, lpSceneMng.GetScreenSize().y / 2 };
		//�}�E�X�̈ʒu���擾
		GetMousePoint(&mousePos_.x, &mousePos_.y);

		//�}�E�X�̈ړ��������v�Z
		Vector2 mouseMoveDistance = { static_cast<float>(mousePos_.x - middleMousePos.x),static_cast<float>(mousePos_.y - middleMousePos.y) };

		//�}�E�X�ʒu�̍����ŉ�]������
		angle_.y += Deg2RadF(mouseMoveDistance.x / 30.0f);	//���E��]
		angle_.x += Deg2RadF(mouseMoveDistance.y / 30.0f);	//�㉺��]

		//������̊p�x����
		if (angle_.x < UP_RADIAN_LIMIT_X)
		{
			angle_.x = UP_RADIAN_LIMIT_X;
		}

		//�������̊p�x����
		if (angle_.x > DOWN_RADIAN_LIMIT_X)
		{
			angle_.x = DOWN_RADIAN_LIMIT_X;
		}

		if (dashFlag)
		{
			angle_.x += SwingWidth();
		}

		//�}�E�X�̈ʒu��^�񒆂ɌŒ�
		SetMousePoint(middleMousePos.x, middleMousePos.y);
	}

	if (angle_.y != defaultangle_y)
	{
		//����B�e���ATutorialflg��true
		lpTutorialMng.SetTutorialFlg(TutorialProgress::ViewShift, true);
	}
}

void Camera::Update(Vector3 targetPos)//�G�ɔO�͂Ŏ����グ����
{
	//���܂��āA���ɗ�����
	FallHead();
}

void Camera::SetBeforeDraw(Vector3 playerpos)
{
	// �N���b�v�����̐ݒ�
	SetCameraNearFar(CAMERA_RANGE_NEAR, CAMERA_RANGE_FAR);

	//�J�����̈ړ�
	//�v���C���[�̈ʒu���J�������W�ɐݒ�
	pos_ = playerpos;
	//pos_.z -= 2200.0f;
	pos_.y = playerpos.y + CAMERA_HEIGHT + offset_.y;

	//�����_�̈ړ�
	//�N�H�[�^�j�I���ɕϊ�
	quaRot_ = Quaternion::Euler(angle_.toVECTOR());
	//���W����](�J��������̑��΍��W)
	Vector3 localPos = ConvertVECTORtoVector3(quaRot_.PosAxis(VGet(0.0f, 0.0f, TARGET2CAMERA_DISTANCE)));
	targetPos_ = pos_ + localPos;

	//�J�����̈ʒu�ƒ����_�̐ݒ�𔽉f
	SetCameraPositionAndTarget_UpVecY(
		pos_.toVECTOR(),
		targetPos_.toVECTOR());
	Effekseer_Sync3DSetting();

}

void Camera::SetBeforeDraw_dead(Vector3 playerpos, Vector3 targetpos)
{
	// �N���b�v�����̐ݒ�
	SetCameraNearFar(CAMERA_RANGE_NEAR, CAMERA_RANGE_FAR);

	//�J�����̈ړ�
	//�v���C���[�̈ʒu���J�������W�ɐݒ�
	pos_ = playerpos;
	//pos_.z -= 2200.0f;
	pos_.y = playerpos.y + CAMERA_HEIGHT + offset_.y;

	//�����_
	//�G�̍��W�{�����Ɏ��_�����킹��
	targetPos_ = targetpos + t_offset_;

	//�J�����̈ʒu�ƒ����_�ƃJ�����̏������ݒ�
	SetCameraPositionAndTargetAndUpVec(
		pos_.toVECTOR(),
		targetPos_.toVECTOR(),
		VGet(0.0f, 1.0f, roll_));
	Effekseer_Sync3DSetting();

}

void Camera::Draw(void)
{
	//�f�o�b�O�\��
#ifdef _DEBUG
	DrawFormatString(600, 0, 0xffffff, "�J�������W : (%.1f, %.1f, %.1f)", pos_.x, pos_.y, pos_.z);
#endif // _DEBUG

}

void Camera::SetSensitivity(float sens)
{
	sensitivity_ = sens;
}

Vector3 Camera::GetPos(void)
{
	return pos_;
}

Vector3 Camera::GetTargetPos()
{
	return targetPos_;
}

Vector3 Camera::GetAngles()
{
	return angle_;
}

float Camera::SwingWidth(void)
{
	auto addSwing = sin(sinCnt_ / SWING_OFFSET) * SWING_WIDTH;
	sinCnt_++;
	return addSwing;
}

void Camera::FallHead(void)
{
	
	if (BreakHead())	//���܂�(�J���������̊p�x��)
	{
		if (!WaitTime(1.0))
		{
			return;
		}

		// ���̂̈ʒu = �����x * ���� + 1 / 2( �����x * ���Ԃ̓�� ) 
		//y(pos.y) = vo(�����x)t(����)�|1 / 2 * g(accel) * t^2

		//�������x(�ŏ��͏�ɏ㏸���A���Ԍo�߂ŗ���)
		float downSpeed = static_cast<float>(
			(-0 * fallCnt_) +
			((1.0 / 2.0) * (fallAccel_ * pow(fallCnt_, 2.0))));

		offset_.y -= downSpeed;
		t_offset_.y -= downSpeed;
			

		//���������x���Z
		fallAccel_ += 9.8;

		if (offset_.y < ZERO - CAMERA_HEIGHT + 300)
		{
			offset_.y = ZERO - CAMERA_HEIGHT + 300;
		}
		if (t_offset_.y < ZERO - CAMERA_HEIGHT-500)
		{
			t_offset_.y = ZERO - CAMERA_HEIGHT-500;
				
		}
	}
	fallCnt_ += lpSceneMng.GetDeltaTime();
	
}

bool Camera::BreakHead(void)
{
	if (roll_ < ROLL_LIMIT)
	{
		
		lpSoundMng.PlayingSound("resource/sound/GameOverSE.mp3");
		//rotate3.h -= DX_PI_F / 60.0f;
		roll_ += ROLL_SPEED;
	}
	else
	{
		return true;
	}
	return false;
}

bool Camera::WaitTime(double waittime)
{
	//�w�肵���҂����Ԃ��o�߂�����true��Ԃ�
	return (waitTime_ += lpSceneMng.GetDeltaTime()) > waittime;
}