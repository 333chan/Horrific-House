#include "../../SceneManager.h"
#include "../../../common/manager/CBufferMng.h"
#include "TitleCBuffer.h"
#include "../../../common/DebugMoveKey.h"

//�J����
constexpr int CBUFFER_CAMERA = 8;
constexpr int CBUFFER_CAMERA_SIZE = 8;
//�X�|�b�g���C�g
constexpr int CBUFFER_SPOTLIGHT = 9;
constexpr int CBUFFER_SPOTLIGHT_SIZE = 16;

constexpr float SPOTLIGHT_OFFSET_X = 120.0f;
constexpr float SPOTLIGHT_OFFSET_Y = 100.0f;
constexpr float SPOTLIGHT_COLOR = 10.0f;
constexpr float SPOTLIGHT_RANGE = 16000.0f;
constexpr double SPOTLIGHT_ANGLE_MAX = 20.0;
constexpr int SPOTLIGHT_ANGLE_MIN = 15;

constexpr int SPOTLIGHT_BETWEEN = 150;

constexpr double LIGHT_TIME_MUL = 300.0;

//���C�g�̓_��
constexpr double LIGHT_FLAS_MUL = 30.0;
constexpr int LIGHT_FLAS_PERCENT = 10;
constexpr int LIGHT_FLAS_EQUAL = 7;

TitleCBuffer::TitleCBuffer(Vector3 cameraPos)
{
	Init(cameraPos);
}

TitleCBuffer::~TitleCBuffer()
{
}

void TitleCBuffer::Init(Vector3 cameraPos)
{
	
	//�o�b�t�@�̏�����
	lpCBufferMng.Remove();
	lpCBufferMng.Create();
	cameraPos_ = cameraPos;
	//�J�����o�b�t�@�̍쐬
	lpCBufferMng.CreatePixelCbuffer(CBUFFER_CAMERA_SIZE);

	//�o�b�t�@�֑��
	lpCBufferMng.SetPixelCbuffer(CBUFFER_CAMERA, 0,
		{ cameraPos.x,
		cameraPos.y,
		cameraPos.z,
		0 });

	//���C�g�o�b�t�@�̍쐬
	lpCBufferMng.CreatePixelCbuffer(CBUFFER_SPOTLIGHT_SIZE);

	//���C�g�̏�����
	titleLight_.SpotLights.Position = cameraPos;

	titleLight_.SpotLights.Color.x = SPOTLIGHT_COLOR;
	titleLight_.SpotLights.Color.y = SPOTLIGHT_COLOR;
	titleLight_.SpotLights.Color.z = SPOTLIGHT_COLOR;

	titleLight_.SpotLights.Direction.x = 0.0f;
	titleLight_.SpotLights.Direction.y = 0.0f;
	titleLight_.SpotLights.Direction.z = 0.0f;

	titleLight_.SpotLights.Direction.Normalize();

	titleLight_.SpotLights.Range = SPOTLIGHT_RANGE;
	titleLight_.SpotLights.Angle = Deg2RadF(SPOTLIGHT_ANGLE_MIN);

	titleLight_.SpotLights.pad0 = 0.0f;
	int i = 0;
	//���C�g�o�b�t�@�ɒl�̑��
	lpCBufferMng.SetPixelCbuffer(CBUFFER_SPOTLIGHT, i,
		{ titleLight_.SpotLights.Position.x ,
		titleLight_.SpotLights.Position.y,
		titleLight_.SpotLights.Position.z,
		titleLight_.SpotLights.pad0 });
	i++;
	lpCBufferMng.SetPixelCbuffer(CBUFFER_SPOTLIGHT, i,
		{ titleLight_.SpotLights.Color.x ,
		titleLight_.SpotLights.Color.y,
		titleLight_.SpotLights.Color.z,
		titleLight_.SpotLights.Range });
	i++;
	lpCBufferMng.SetPixelCbuffer(CBUFFER_SPOTLIGHT, i,
		{ titleLight_.SpotLights.Direction.x ,
		titleLight_.SpotLights.Direction.y,
		titleLight_.SpotLights.Direction.z,
		titleLight_.SpotLights.Angle });

	//���C�g�̊p�x
	lightAngle_ = SPOTLIGHT_ANGLE_MAX;
	//���C�g�̖��邳��������
	lightPower_ = 0;
	StartCnt_ = 0;
	//�}�E�X�擾
	GetMousePoint(&mousePos_.x, &mousePos_.y);
}

void TitleCBuffer::Update()
{
	//�f���^�^�C���𑫂�
	StartCnt_ += lpSceneMng.GetDeltaTime();
	if (StartCnt_ > 1.0)
	{
		//��b�������烉�C�g������
		lightPower_ = SPOTLIGHT_COLOR;
	}
	else
	{
		//���C�g��_�ł�����
		if (static_cast<int>(StartCnt_ * LIGHT_FLAS_MUL) % LIGHT_FLAS_PERCENT > LIGHT_FLAS_EQUAL)
		{
			lightPower_ = SPOTLIGHT_COLOR;
		}
		else
		{
			lightPower_ = 0;
		}

	}
	lightAngle_ -= lpSceneMng.GetDeltaTime() * LIGHT_TIME_MUL;
	//���ׂẴo�b�t�@�̍X�V
	lpCBufferMng.Update();
	//�O�̍��W������
	oldMousePos_ = mousePos_;
	//�}�E�X���W�̍X�V
	GetMousePoint(&mousePos_.x, &mousePos_.y);
	//�}�E�X�|�C���^���W���g����3D���W���쐬
	auto mouseVector3 = ConvertVECTORtoVector3(
		ConvScreenPosToWorldPos(
			{ static_cast<float>(mousePos_.x - SPOTLIGHT_OFFSET_X), static_cast<float>(mousePos_.y - SPOTLIGHT_OFFSET_Y),1.0f }
	));

	titleLight_.SpotLights.Direction = mouseVector3 - cameraPos_;
	titleLight_.SpotLights.Direction.Normalize();
	//�X�|�b�g���C�g�F�X�V
	titleLight_.SpotLights.Color.x = static_cast<float>(lightPower_);
	titleLight_.SpotLights.Color.y = static_cast<float>(lightPower_);
	titleLight_.SpotLights.Color.z = static_cast<float>(lightPower_);

	float color0 = 0.0f;
	//���C�g��_�ł����Ă���
	if (std::rand() % SPOTLIGHT_BETWEEN == 0)
	{
		titleLight_.SpotLights.Color = { color0,color0,color0 };
	}
	else
	{
		titleLight_.SpotLights.Color = { static_cast<float>(lightPower_),static_cast<float>(lightPower_),static_cast<float>(lightPower_)};
	}
	//�X�|�b�g���C�g�̊p�x
	titleLight_.SpotLights.Angle = Deg2RadF(static_cast<float>((std::max)(SPOTLIGHT_ANGLE_MIN, static_cast<int>(lightAngle_))));

	//���C�g�o�b�t�@�̑��
	int i = 0;
	lpCBufferMng.SetPixelCbuffer(CBUFFER_SPOTLIGHT, i,
		{ titleLight_.SpotLights.Position.x ,
		titleLight_.SpotLights.Position.y,
		titleLight_.SpotLights.Position.z,
		0 });
	i++;
	lpCBufferMng.SetPixelCbuffer(CBUFFER_SPOTLIGHT, i ,
		{ titleLight_.SpotLights.Color.x ,
		titleLight_.SpotLights.Color.y,
		titleLight_.SpotLights.Color.z,
		titleLight_.SpotLights.Range });
	i++;
	lpCBufferMng.SetPixelCbuffer(CBUFFER_SPOTLIGHT, i,
		{ titleLight_.SpotLights.Direction.x ,
		titleLight_.SpotLights.Direction.y ,
		titleLight_.SpotLights.Direction.z ,
		titleLight_.SpotLights.Angle });
	lpCBufferMng.Update();

#ifdef _DEBUG
	// �p�����[�^���쏈��
#endif	//_DEBUG

}