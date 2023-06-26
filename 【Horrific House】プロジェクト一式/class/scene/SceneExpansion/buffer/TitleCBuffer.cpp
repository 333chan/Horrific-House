#include "../../SceneManager.h"
#include "../../../common/manager/CBufferMng.h"
#include "TitleCBuffer.h"
#include "../../../common/DebugMoveKey.h"

//カメラ
constexpr int CBUFFER_CAMERA = 8;
constexpr int CBUFFER_CAMERA_SIZE = 8;
//スポットライト
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

//ライトの点滅
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
	
	//バッファの初期化
	lpCBufferMng.Remove();
	lpCBufferMng.Create();
	cameraPos_ = cameraPos;
	//カメラバッファの作成
	lpCBufferMng.CreatePixelCbuffer(CBUFFER_CAMERA_SIZE);

	//バッファへ代入
	lpCBufferMng.SetPixelCbuffer(CBUFFER_CAMERA, 0,
		{ cameraPos.x,
		cameraPos.y,
		cameraPos.z,
		0 });

	//ライトバッファの作成
	lpCBufferMng.CreatePixelCbuffer(CBUFFER_SPOTLIGHT_SIZE);

	//ライトの初期化
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
	//ライトバッファに値の代入
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

	//ライトの角度
	lightAngle_ = SPOTLIGHT_ANGLE_MAX;
	//ライトの明るさを初期化
	lightPower_ = 0;
	StartCnt_ = 0;
	//マウス取得
	GetMousePoint(&mousePos_.x, &mousePos_.y);
}

void TitleCBuffer::Update()
{
	//デルタタイムを足す
	StartCnt_ += lpSceneMng.GetDeltaTime();
	if (StartCnt_ > 1.0)
	{
		//一秒超えたらライトをつける
		lightPower_ = SPOTLIGHT_COLOR;
	}
	else
	{
		//ライトを点滅させる
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
	//すべてのバッファの更新
	lpCBufferMng.Update();
	//前の座標を入れる
	oldMousePos_ = mousePos_;
	//マウス座標の更新
	GetMousePoint(&mousePos_.x, &mousePos_.y);
	//マウスポインタ座標を使って3D座標を作成
	auto mouseVector3 = ConvertVECTORtoVector3(
		ConvScreenPosToWorldPos(
			{ static_cast<float>(mousePos_.x - SPOTLIGHT_OFFSET_X), static_cast<float>(mousePos_.y - SPOTLIGHT_OFFSET_Y),1.0f }
	));

	titleLight_.SpotLights.Direction = mouseVector3 - cameraPos_;
	titleLight_.SpotLights.Direction.Normalize();
	//スポットライト色更新
	titleLight_.SpotLights.Color.x = static_cast<float>(lightPower_);
	titleLight_.SpotLights.Color.y = static_cast<float>(lightPower_);
	titleLight_.SpotLights.Color.z = static_cast<float>(lightPower_);

	float color0 = 0.0f;
	//ライトを点滅させている
	if (std::rand() % SPOTLIGHT_BETWEEN == 0)
	{
		titleLight_.SpotLights.Color = { color0,color0,color0 };
	}
	else
	{
		titleLight_.SpotLights.Color = { static_cast<float>(lightPower_),static_cast<float>(lightPower_),static_cast<float>(lightPower_)};
	}
	//スポットライトの角度
	titleLight_.SpotLights.Angle = Deg2RadF(static_cast<float>((std::max)(SPOTLIGHT_ANGLE_MIN, static_cast<int>(lightAngle_))));

	//ライトバッファの代入
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
	// パラメータ操作処理
#endif	//_DEBUG

}