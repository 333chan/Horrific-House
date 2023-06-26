#include "Stamina.h"
#include "../../scene/SceneManager.h"
#include "../../common/manager/ImageMng.h"
#include "../../input/PadInput.h"
#include "../../input/KeyInput.h"
#include "../../../_debug/_DebugDispOut.h"
#include "../../common/manager/SoundMng.h"

#ifdef _DEBUG
constexpr float DASH_TIME_LIMIT = 700.0f;	//�_�b�V���\�Ȏ���
#else
constexpr float DASH_TIME_LIMIT = 7.0f;	//�_�b�V���\�Ȏ���
#endif

constexpr float DASH_ADD_TIME = 5.0f;		//�_�b�V�����Ԃ��߂����ꍇ�ɒǉ�����b��
constexpr float DASH_COEFFIECIENT = 2.75f;	//�_�b�V�������ꍇ�̑��x�{��
constexpr float FATIGUE_COEFFICIENT = 0.4f;	//��J��Ԃ̑��x�{��
constexpr float DONT_DASH_HEAL_COEFFICIENT = 3.0f;	//�ړ����Ă��Ȃ��ꍇ�̃X�^�~�i�񕜑��x
constexpr float HEAL_COEFFICIENT = 2.0f;	//�ړ����Ă��Ȃ��ꍇ�̃X�^�~�i�񕜑��x

//�X�^�~�i�Q�[�W�֌W
constexpr int STAMINA_GAUGE_HEIGHT = 12;	//�Q�[�W�̍���
constexpr int STAMINA_GAUGE_WIDTH = 300;	//�Q�[�W�̕�

constexpr int STAMINA_GAUGE_COLOR_GREEN  = 0x44ff44;	//�X�^�~�i���������̐F(��)
constexpr int STAMINA_GAUGE_COLOR_YELLOW = 0xffaa22;	//�X�^�~�i�������Ă������̐F(��)
constexpr int STAMINA_GAUGE_COLOR_RED    = 0xff4444;	//�X�^�~�i���قƂ�ǂȂ����̐F(��)

constexpr int STAMINA_GAUGE_OFFSET_Y = 100;			//�X�^�~�i�̏c�̃I�t�Z�b�g
constexpr int STAMINA_GAUGE_ALPHA_MAX = 255;		//�X�^�~�i�̃A���t�@�ő�l
constexpr int STAMINA_GAUGE_ALPHA_MINUS = 5;		//�X�^�~�i�̖��t���[���̃}�C�i�X�l
constexpr double STAMINA_GAUGE_TRANS_LIMIT = 0.3;	//�Q�[�W�����߂��鎞��

Stamina::Stamina(ControllerType type)
{
	//�R���g���[������
	if (type == ControllerType::Pad)
	{
		controller_ = std::make_unique<PadInput>();
	}
	else
	{
		controller_ = std::make_unique<KeyInput>();
	}
	Init();
}

Stamina::~Stamina()
{
}

void Stamina::Init(void)
{
	//�ړ��W��
	moveCoefficient_ = 1.0f;
	//�_�b�V������
	dashTime_ = 0.0;
	//������Ԃɂ���
	gaugeAlpha_ = 0;
	//�����͗ΐF
	gaugeColor = STAMINA_GAUGE_COLOR_GREEN;
	//�_�b�V���t���O(�؂�ւ��p)
	dashFlag_ = false;
}

void Stamina::Release(void)
{
}

float Stamina::Update(bool isCamera)
{
	isCamera_ = isCamera;
	controller_->Update();

	moveCoefficient_ = 1.0f;

	//���l����
	CalcStaminaGaugeAlpha(true);

	//�F�ύX����
	ChangeStaminaGaugeColor();

	//�ړ��W���Ǝ��ԏ���
	CalcCoefficientAndTime();

	return moveCoefficient_;
}

void Stamina::Draw(void)
{
	//��J���͂��΂炭�\�����Ȃ�
	int lineWidth = DASH_TIME_LIMIT - dashTime_ >= 0.0f ? static_cast<int>(STAMINA_GAUGE_WIDTH * ((DASH_TIME_LIMIT - dashTime_) / DASH_TIME_LIMIT)) : 0;

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, gaugeAlpha_);

	//�X�^�~�i�Q�[�W(�E)
	DrawLine(
		lpSceneMng.GetScreenSize().x / 2,
		lpSceneMng.GetScreenSize().y - STAMINA_GAUGE_OFFSET_Y,
		lpSceneMng.GetScreenSize().x / 2 + lineWidth,
		lpSceneMng.GetScreenSize().y - STAMINA_GAUGE_OFFSET_Y,
		gaugeColor,
		STAMINA_GAUGE_HEIGHT
	);

	//�X�^�~�i�Q�[�W(��)
	DrawLine(
		lpSceneMng.GetScreenSize().x / 2,
		lpSceneMng.GetScreenSize().y - STAMINA_GAUGE_OFFSET_Y,
		lpSceneMng.GetScreenSize().x / 2 - lineWidth,
		lpSceneMng.GetScreenSize().y - STAMINA_GAUGE_OFFSET_Y,
		gaugeColor,
		STAMINA_GAUGE_HEIGHT
	);

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, STAMINA_GAUGE_ALPHA_MAX);
}

void Stamina::CalcCoefficientAndTime(void)
{
	//��J(fatigue)���Ă��邩�ǂ���
	bool fatigueFlag = DASH_TIME_LIMIT - dashTime_ < 0.0f;

	//��J���Ă���ꍇ�A��莞�ԑ���x������
	if (fatigueFlag)
	{
		moveCoefficient_ *= FATIGUE_COEFFICIENT;
		lpSoundMng.PlayingSound("./resource/sound/PlayerRunOutVoice.mp3");
	}
	else
	{
		lpSoundMng.StopSound("./resource/sound/PlayerRunOutVoice.mp3");
	}

	//�؂�ւ��������ꂽ�ꍇ
	if (controller_->CheckInputKey(KeyID::DashForAndroid))
	{
		dashFlag_ = !dashFlag_;
	}

	//�X�^�~�i�񕜌W��
	//�X�^�~�i���؂�Ă���Ƃ��͉񕜂��n�߂�܂�1�{
	double healSpeed = DASH_TIME_LIMIT - dashTime_ >= 0.0 ? HEAL_COEFFICIENT : 1.0;

	//�ړ����Ă��Ȃ��ꍇ�̓X�^�~�i�񕜂��ĕԂ�
	if (!controller_->CheckLongInputKey(KeyID::Up) &&
		!controller_->CheckLongInputKey(KeyID::Down) &&
		!controller_->CheckLongInputKey(KeyID::Right) &&
		!controller_->CheckLongInputKey(KeyID::Left)
		)
	{
		//�ړ����Ă��Ȃ��̂ŃX�^�~�i�񕜗ʃA�b�v
		dashTime_ -= lpSceneMng.GetDeltaTime() / healSpeed * DONT_DASH_HEAL_COEFFICIENT;
		if (dashTime_ < 0.0)
		{
			dashTime_ = 0.0;
		}
		return;
	}
	auto KeyDashFlag_ = controller_->CheckLongInputKey(KeyID::Dash);
	if (isCamera_)
	{
		KeyDashFlag_ = false;
	}
	//�_�b�V�������Ă���ꍇ�͌W����傫������
	if ((KeyDashFlag_ && !fatigueFlag) || (dashFlag_ && !fatigueFlag))
	{
		dashTime_ += lpSceneMng.GetDeltaTime();		//�_�b�V�����ԃJ�E���g

		//�X�^�~�i��\��������
		CalcStaminaGaugeAlpha(false);

		if (dashTime_ <= DASH_TIME_LIMIT)
		{
			moveCoefficient_ *= DASH_COEFFIECIENT;
		}
		else
		{
			//�X�^�~�i���؂ꂽ�ꍇ�͉񕜂���܂ł̎��Ԃ�����������
			dashTime_ = DASH_TIME_LIMIT + DASH_ADD_TIME;
		}
	}
	else
	{
		//�_�b�V�����Ă��Ȃ��ꍇ�̓X�^�~�i���񕜂�����
		dashTime_ -= lpSceneMng.GetDeltaTime() / healSpeed;	
		if (dashTime_ < 0.0)
		{
			dashTime_ = 0.0;
		}
	}
}

void Stamina::CalcStaminaGaugeAlpha(bool transFlag)
{
	//true�̏ꍇ�͓��߂���
	if (transFlag)
	{
		if (dashTime_ < STAMINA_GAUGE_TRANS_LIMIT)
		{
			//�X�^�~�i��������x�񕜂�����Q�[�W�����X�ɓ��߂�����
			if (gaugeAlpha_ > 0)
			{
				gaugeAlpha_ -= STAMINA_GAUGE_ALPHA_MINUS;
			}
		}
	}
	else
	{
		gaugeAlpha_ = STAMINA_GAUGE_ALPHA_MAX;
	}
}

void Stamina::ChangeStaminaGaugeColor(void)
{
	//�_�b�V���ł���c��̎��Ԃ������ȏ�̏ꍇ�͗΁A������艺�̏ꍇ�͉��F�A�S���̂P��艺�̏ꍇ�͐ԐF�ɂ���
	if (DASH_TIME_LIMIT - dashTime_ >= DASH_TIME_LIMIT / 2.0f)
	{
		gaugeColor = STAMINA_GAUGE_COLOR_GREEN;
	}
	else if(DASH_TIME_LIMIT - dashTime_ >= DASH_TIME_LIMIT / 4.0f)
	{
		gaugeColor = STAMINA_GAUGE_COLOR_YELLOW;
	}
	else
	{
		gaugeColor = STAMINA_GAUGE_COLOR_RED;
	}
}
