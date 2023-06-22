#include "Stamina.h"
#include "../../scene/SceneManager.h"
#include "../../common/manager/ImageMng.h"
#include "../../input/PadInput.h"
#include "../../input/KeyInput.h"
#include "../../../_debug/_DebugDispOut.h"
#include "../../common/manager/SoundMng.h"

#ifdef _DEBUG
constexpr float DASH_TIME_LIMIT = 700.0f;	//ダッシュ可能な時間
#else
constexpr float DASH_TIME_LIMIT = 7.0f;	//ダッシュ可能な時間
#endif

constexpr float DASH_ADD_TIME = 5.0f;		//ダッシュ時間を過ぎた場合に追加する秒数
constexpr float DASH_COEFFIECIENT = 2.75f;	//ダッシュした場合の速度倍率
constexpr float FATIGUE_COEFFICIENT = 0.4f;	//疲労状態の速度倍率
constexpr float DONT_DASH_HEAL_COEFFICIENT = 3.0f;	//移動していない場合のスタミナ回復速度
constexpr float HEAL_COEFFICIENT = 2.0f;	//移動していない場合のスタミナ回復速度

//スタミナゲージ関係
constexpr int STAMINA_GAUGE_HEIGHT = 12;	//ゲージの高さ
constexpr int STAMINA_GAUGE_WIDTH = 300;	//ゲージの幅

constexpr int STAMINA_GAUGE_COLOR_GREEN  = 0x44ff44;	//スタミナが多い時の色(緑)
constexpr int STAMINA_GAUGE_COLOR_YELLOW = 0xffaa22;	//スタミナが減ってきた時の色(黄)
constexpr int STAMINA_GAUGE_COLOR_RED    = 0xff4444;	//スタミナがほとんどない時の色(赤)

constexpr int STAMINA_GAUGE_OFFSET_Y = 100;			//スタミナの縦のオフセット
constexpr int STAMINA_GAUGE_ALPHA_MAX = 255;		//スタミナのアルファ最大値
constexpr int STAMINA_GAUGE_ALPHA_MINUS = 5;		//スタミナの毎フレームのマイナス値
constexpr double STAMINA_GAUGE_TRANS_LIMIT = 0.3;	//ゲージが透過する時間

Stamina::Stamina(ControllerType type)
{
	//コントローラ生成
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
	//移動係数
	moveCoefficient_ = 1.0f;
	//ダッシュ時間
	dashTime_ = 0.0;
	//透明状態にする
	gaugeAlpha_ = 0;
	//初期は緑色
	gaugeColor = STAMINA_GAUGE_COLOR_GREEN;
	//ダッシュフラグ(切り替え用)
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

	//α値処理
	CalcStaminaGaugeAlpha(true);

	//色変更処理
	ChangeStaminaGaugeColor();

	//移動係数と時間処理
	CalcCoefficientAndTime();

	return moveCoefficient_;
}

void Stamina::Draw(void)
{
	//疲労中はしばらく表示しない
	int lineWidth = DASH_TIME_LIMIT - dashTime_ >= 0.0f ? static_cast<int>(STAMINA_GAUGE_WIDTH * ((DASH_TIME_LIMIT - dashTime_) / DASH_TIME_LIMIT)) : 0;

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, gaugeAlpha_);

	//スタミナゲージ(右)
	DrawLine(
		lpSceneMng.GetScreenSize().x / 2,
		lpSceneMng.GetScreenSize().y - STAMINA_GAUGE_OFFSET_Y,
		lpSceneMng.GetScreenSize().x / 2 + lineWidth,
		lpSceneMng.GetScreenSize().y - STAMINA_GAUGE_OFFSET_Y,
		gaugeColor,
		STAMINA_GAUGE_HEIGHT
	);

	//スタミナゲージ(左)
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
	//疲労(fatigue)しているかどうか
	bool fatigueFlag = DASH_TIME_LIMIT - dashTime_ < 0.0f;

	//疲労している場合、一定時間足を遅くする
	if (fatigueFlag)
	{
		moveCoefficient_ *= FATIGUE_COEFFICIENT;
		lpSoundMng.PlayingSound("./resource/sound/PlayerRunOutVoice.mp3");
	}
	else
	{
		lpSoundMng.StopSound("./resource/sound/PlayerRunOutVoice.mp3");
	}

	//切り替えが押された場合
	if (controller_->CheckInputKey(KeyID::DashForAndroid))
	{
		dashFlag_ = !dashFlag_;
	}

	//スタミナ回復係数
	//スタミナが切れているときは回復し始めるまで1倍
	double healSpeed = DASH_TIME_LIMIT - dashTime_ >= 0.0 ? HEAL_COEFFICIENT : 1.0;

	//移動していない場合はスタミナ回復して返す
	if (!controller_->CheckLongInputKey(KeyID::Up) &&
		!controller_->CheckLongInputKey(KeyID::Down) &&
		!controller_->CheckLongInputKey(KeyID::Right) &&
		!controller_->CheckLongInputKey(KeyID::Left)
		)
	{
		//移動していないのでスタミナ回復量アップ
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
	//ダッシュをしている場合は係数を大きくする
	if ((KeyDashFlag_ && !fatigueFlag) || (dashFlag_ && !fatigueFlag))
	{
		dashTime_ += lpSceneMng.GetDeltaTime();		//ダッシュ時間カウント

		//スタミナを表示させる
		CalcStaminaGaugeAlpha(false);

		if (dashTime_ <= DASH_TIME_LIMIT)
		{
			moveCoefficient_ *= DASH_COEFFIECIENT;
		}
		else
		{
			//スタミナが切れた場合は回復するまでの時間を延長させる
			dashTime_ = DASH_TIME_LIMIT + DASH_ADD_TIME;
		}
	}
	else
	{
		//ダッシュしていない場合はスタミナを回復させる
		dashTime_ -= lpSceneMng.GetDeltaTime() / healSpeed;	
		if (dashTime_ < 0.0)
		{
			dashTime_ = 0.0;
		}
	}
}

void Stamina::CalcStaminaGaugeAlpha(bool transFlag)
{
	//trueの場合は透過する
	if (transFlag)
	{
		if (dashTime_ < STAMINA_GAUGE_TRANS_LIMIT)
		{
			//スタミナがある程度回復したらゲージを徐々に透過させる
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
	//ダッシュできる残りの時間が半分以上の場合は緑、半分より下の場合は黄色、４分の１より下の場合は赤色にする
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
