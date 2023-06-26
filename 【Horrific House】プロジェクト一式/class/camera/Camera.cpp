#include<DxLib.h>
#include<EffekseerForDXLib.h>
#include"../Scene/SceneManager.h"
#include "Camera.h"
#include "../common/manager/SoundMng.h"
#include "../common/manager/TutorialMng.h"

//注視点からカメラまでの距離
constexpr float TARGET2CAMERA_DISTANCE = 500.0f;

//X軸回転時の制限角度
constexpr float UP_RADIAN_LIMIT_X  = -70.0f * (DX_PI_F / 180.0f);	//上方向
constexpr float DOWN_RADIAN_LIMIT_X = 60.0f * (DX_PI_F / 180.0f);	//下方向 60

//カメラの描画距離
constexpr float CAMERA_RANGE_NEAR = 50.0f;		//手前
constexpr float CAMERA_RANGE_FAR  = 12500.0f;	//奥

//画面揺れ
constexpr float SWING_WIDTH = 0.125f * (DX_PI_F / 180.0f);
constexpr float SWING_OFFSET = 3.5f;

//死亡演出時のカメラの制限
constexpr float LIFT_HEIGHT = 800.0f;	//敵がプレイヤーを持ち上げる時の高さ
constexpr float LIFT_SPEED = 8.0f;	//上昇速度
constexpr float ZERO = 0.0f;	//0
constexpr float ROLL_LIMIT = 6.0f;	//回転の限界値
constexpr float ROLL_SPEED = 60.0f * (DX_PI_F / 180.0f);	//回転速度

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
	//位置
	pos_ = { 0.0f,CAMERA_HEIGHT,-TARGET2CAMERA_DISTANCE };
	offset_ = { 0.0f,0.0f,0.0f };
	t_offset_ = { 0.0f,1400.0f,0.0f };
	//角度
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
	// クリップ距離の設定
	SetCameraNearFar(CAMERA_RANGE_NEAR, CAMERA_RANGE_FAR);

	//マウスの位置を真ん中に固定
	SetMousePoint(lpSceneMng.GetScreenSize().x / 2, lpSceneMng.GetScreenSize().y / 2);
}

void Camera::Update(bool dashFlag)
{
	if (GetJoypadNum())
	{
		//PAD接続中
		DINPUT_JOYSTATE rStick_Input;
		//右スティックの入力状態を取得
		GetJoypadDirectInputState(DX_INPUT_PAD1, &rStick_Input);

		Vector2Int middlePadPos = { lpSceneMng.GetScreenSize().x / 2, lpSceneMng.GetScreenSize().y / 2 };
		
		padPos_.x += rStick_Input.Rx / sensitivity_;
		padPos_.y += rStick_Input.Ry / sensitivity_;

		//マウスの移動距離を計算
		Vector2 padMoveDistance = { static_cast<float>(padPos_.x - middlePadPos.x),static_cast<float>(padPos_.y - middlePadPos.y) };

		//マウス位置の差分で回転させる
		angle_.y += Deg2RadF(padMoveDistance.x / 30.0f);	//左右回転
		angle_.x += Deg2RadF(padMoveDistance.y / 30.0f);	//上下回転
		//上方向の角度制限
		if (angle_.x < UP_RADIAN_LIMIT_X)
		{
			angle_.x = UP_RADIAN_LIMIT_X;
		}

		//下方向の角度制限
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
		//マウスの位置を取得
		GetMousePoint(&mousePos_.x, &mousePos_.y);

		//マウスの移動距離を計算
		Vector2 mouseMoveDistance = { static_cast<float>(mousePos_.x - middleMousePos.x),static_cast<float>(mousePos_.y - middleMousePos.y) };

		//マウス位置の差分で回転させる
		angle_.y += Deg2RadF(mouseMoveDistance.x / 30.0f);	//左右回転
		angle_.x += Deg2RadF(mouseMoveDistance.y / 30.0f);	//上下回転

		//上方向の角度制限
		if (angle_.x < UP_RADIAN_LIMIT_X)
		{
			angle_.x = UP_RADIAN_LIMIT_X;
		}

		//下方向の角度制限
		if (angle_.x > DOWN_RADIAN_LIMIT_X)
		{
			angle_.x = DOWN_RADIAN_LIMIT_X;
		}

		if (dashFlag)
		{
			angle_.x += SwingWidth();
		}

		//マウスの位置を真ん中に固定
		SetMousePoint(middleMousePos.x, middleMousePos.y);
	}

	if (angle_.y != defaultangle_y)
	{
		//初回撮影時、Tutorialflgをtrue
		lpTutorialMng.SetTutorialFlg(TutorialProgress::ViewShift, true);
	}
}

void Camera::Update(Vector3 targetPos)//敵に念力で持ち上げられる
{
	//首を折られて、下に落ちる
	FallHead();
}

void Camera::SetBeforeDraw(Vector3 playerpos)
{
	// クリップ距離の設定
	SetCameraNearFar(CAMERA_RANGE_NEAR, CAMERA_RANGE_FAR);

	//カメラの移動
	//プレイヤーの位置をカメラ座標に設定
	pos_ = playerpos;
	//pos_.z -= 2200.0f;
	pos_.y = playerpos.y + CAMERA_HEIGHT + offset_.y;

	//注視点の移動
	//クォータニオンに変換
	quaRot_ = Quaternion::Euler(angle_.toVECTOR());
	//座標を回転(カメラからの相対座標)
	Vector3 localPos = ConvertVECTORtoVector3(quaRot_.PosAxis(VGet(0.0f, 0.0f, TARGET2CAMERA_DISTANCE)));
	targetPos_ = pos_ + localPos;

	//カメラの位置と注視点の設定を反映
	SetCameraPositionAndTarget_UpVecY(
		pos_.toVECTOR(),
		targetPos_.toVECTOR());
	Effekseer_Sync3DSetting();

}

void Camera::SetBeforeDraw_dead(Vector3 playerpos, Vector3 targetpos)
{
	// クリップ距離の設定
	SetCameraNearFar(CAMERA_RANGE_NEAR, CAMERA_RANGE_FAR);

	//カメラの移動
	//プレイヤーの位置をカメラ座標に設定
	pos_ = playerpos;
	//pos_.z -= 2200.0f;
	pos_.y = playerpos.y + CAMERA_HEIGHT + offset_.y;

	//注視点
	//敵の座標＋高さに視点を合わせる
	targetPos_ = targetpos + t_offset_;

	//カメラの位置と注視点とカメラの上方向を設定
	SetCameraPositionAndTargetAndUpVec(
		pos_.toVECTOR(),
		targetPos_.toVECTOR(),
		VGet(0.0f, 1.0f, roll_));
	Effekseer_Sync3DSetting();

}

void Camera::Draw(void)
{
	//デバッグ表示
#ifdef _DEBUG
	DrawFormatString(600, 0, 0xffffff, "カメラ座標 : (%.1f, %.1f, %.1f)", pos_.x, pos_.y, pos_.z);
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
	
	if (BreakHead())	//首を折る(カメラを一定の角度回す)
	{
		if (!WaitTime(1.0))
		{
			return;
		}

		// 物体の位置 = 初速度 * 時間 + 1 / 2( 加速度 * 時間の二乗 ) 
		//y(pos.y) = vo(初速度)t(時間)－1 / 2 * g(accel) * t^2

		//落下速度(最初は上に上昇し、時間経過で落下)
		float downSpeed = static_cast<float>(
			(-0 * fallCnt_) +
			((1.0 / 2.0) * (fallAccel_ * pow(fallCnt_, 2.0))));

		offset_.y -= downSpeed;
		t_offset_.y -= downSpeed;
			

		//落下加速度加算
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
	//指定した待ち時間が経過したらtrueを返す
	return (waitTime_ += lpSceneMng.GetDeltaTime()) > waittime;
}