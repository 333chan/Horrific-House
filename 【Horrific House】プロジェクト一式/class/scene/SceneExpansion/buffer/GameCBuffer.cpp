#include "../../../common/manager/CBufferMng.h"
#include "../../../scene/SceneManager.h"
#include "GameCBuffer.h"
#include "../../../common/DebugMoveKey.h"
//デフォルトサイズ
constexpr int CBUFFER_SIZE			= 16;
//頂点
constexpr int CBUFFER_VERTEX		= 5;
constexpr int CBUFFER_VERTEX_SIZE   = 4;
//カメラ
constexpr int CBUFFER_CAMERA		= 6;
constexpr int CBUFFER_CAMERA_SIZE	= 32;
//ライトバッファ
constexpr int CBUFFER_DIRECTIONLIGHT= 9;
constexpr int CBUFFER_POINTLIGHT	= 10;
constexpr int CBUFFER_SPOTLIGHT		= 11;
//アンビエント
constexpr int CBUFFER_AMBIENTLIGHT  = 12;
constexpr int CBUFFER_AMBIENT_SIZE  = 8;
//ポイントライト
constexpr float POINT_POS_X0 = -5500;
constexpr float POINT_POS_Y0 = 2700;
constexpr float POINT_POS_Z0 = -4500;
constexpr float POINT_RANGE0 = 3000;

constexpr float POINT_POS_X1 = 13500;
constexpr float POINT_POS_Y1 = -1000;
constexpr float POINT_POS_Z1 = -19100;

constexpr float POINT_POS_X2 = 7900;
constexpr float POINT_POS_Y2 = 5100;
constexpr float POINT_POS_Z2 = -25500;
constexpr float POINT_RANGE = 1000;

constexpr float POINT_COLOR_ZERO = 0.0f;
constexpr float POINT_COLOR_MAX	= 2.0f;
constexpr int POINT_BETWEEN		= 120;

constexpr float POINT_COLOR_MIN = 6.0f;
constexpr float POINT_COLOR_MAX0 = 10.0f;
//スポットライト
constexpr float SPOT_COLOR_MIN_0	= 2.0f;
constexpr float SPOT_COLOR_MAX_0	= 5.0f;

//プレイヤー
constexpr int PLAYER_LIGHT	= 0;
constexpr int PLAYER_WEAK_LIGHT_LENGTH = 4000;
constexpr float PLAYER_LIGHT_COLOR = 5.0f;
constexpr float PLAYER_LIGHT_RANGE = 13000.0f;
constexpr float PLAYER_LIGHT_ANGLE= 20.0f;

//エネミー
constexpr int ENEMY_LIGHT = 1;
constexpr float ENEMY_LIGHT_POSITION_OFFSET_Y = 1700;
constexpr float ENEMY_LIGHT_DIRECTION_OFFSET_Y = 0.2f;

constexpr float ENEMY_LIGHT_COLOR = 20.0f;
constexpr float ENEMY_LIGHT_RANGE = 10000.0f;
constexpr float ENEMY_LIGHT_ANGLE = 30.0f;

GameCBuffer::GameCBuffer()
{
	Init();
}
GameCBuffer::GameCBuffer(Vector3 defPos,Vector3 defTargetPos)
{
	defPos_ = defPos;
	defTargetPos_ = defTargetPos;
	Init();
}

GameCBuffer::~GameCBuffer()
{
}

void GameCBuffer::Init()
{
	//実装するためにマジックナンバーで配置、後でバイナリデータで配置
	markerPos_.emplace_back(Vector3{-15000.0f, 1335.4f,-4500.0f  });
	markerPos_.emplace_back(Vector3{ 22500.0f, 1336.5f,-7944.0f });
	markerPos_.emplace_back(Vector3{ 20500.0f, 1336.5f,-16000.0f });
	markerPos_.emplace_back(Vector3{  7300.0f, 1335.4f,-19300.0f });
	markerPos_.emplace_back(Vector3{-10300.0f, 5166.2f,-7000.0f });
	markerPos_.emplace_back(Vector3{ 15500.0f, 5166.2f,-6500.0f });
	markerPos_.emplace_back(Vector3{  8800.0f,-2296.9f,-3500.0f });

	//バッファの作成
	lpCBufferMng.Remove();
	lpCBufferMng.Create();
	
	lpCBufferMng.CreatePixelCbuffer(CBUFFER_SIZE);
	//ディフューズライト
	lpCBufferMng.CreatePixelCbuffer(CBUFFER_SIZE * DIRECTION_LIGHT_NUM);
	//ポイントライト
	lpCBufferMng.CreatePixelCbuffer(CBUFFER_SIZE * POINT_LIGHT_NUM);
	//スポットライト
	lpCBufferMng.CreatePixelCbuffer(CBUFFER_SIZE * SPOT_LIGHT_NUM);
	//アンビエント＆すぺきゅらパワー
	lpCBufferMng.CreatePixelCbuffer(CBUFFER_AMBIENT_SIZE);
	//頂点
	lpCBufferMng.CreateVertexCbuffer(CBUFFER_VERTEX, CBUFFER_VERTEX_SIZE);
	//カメラ
	lpCBufferMng.CreateVertexCbuffer(CBUFFER_CAMERA, CBUFFER_CAMERA_SIZE);
	
	//ライトバッファの更新
	int i = 0;
	for (int j = 0; j < DIRECTION_LIGHT_NUM; j++)
	{
		lpCBufferMng.SetPixelCbuffer(CBUFFER_DIRECTIONLIGHT, i,
			{
			light_.directionLights[j].color.x,
			light_.directionLights[j].color.y,
			light_.directionLights[j].color.z,

			light_.directionLights[j].pad0
			});
		i++;
		lpCBufferMng.SetPixelCbuffer(CBUFFER_DIRECTIONLIGHT, i,
			{
			light_.directionLights[j].direction.x ,
			light_.directionLights[j].direction.y,
			light_.directionLights[j].direction.z,

			light_.directionLights[j].pad1
			});
		i++;
	}
	light_.pointLights[0].position = { POINT_POS_X0,POINT_POS_Y0 ,POINT_POS_Z0 };
	light_.pointLights[0].color = { POINT_COLOR_MIN,POINT_COLOR_MIN,POINT_COLOR_MIN };
	light_.pointLights[0].range = POINT_RANGE0;

	//地下
	light_.pointLights[1].position = { POINT_POS_X1, POINT_POS_Y1, POINT_POS_Z1 };
	light_.pointLights[1].color = { POINT_COLOR_MAX/2,SPOT_COLOR_MIN_0,0.0f };
	light_.pointLights[1].range = POINT_RANGE;

	//
	light_.pointLights[2].position = { POINT_POS_X2, POINT_POS_Y2, POINT_POS_Z2 };
	light_.pointLights[2].color = { POINT_COLOR_MAX,POINT_COLOR_MAX,POINT_COLOR_MAX };
	light_.pointLights[2].range = POINT_RANGE;

	i = 0;
	for (int j = 0; j < POINT_LIGHT_NUM; j++)
	{
		lpCBufferMng.SetPixelCbuffer(CBUFFER_POINTLIGHT, i,
			{
			light_.pointLights[j].position.x,
			light_.pointLights[j].position.y,
			light_.pointLights[j].position.z,

			light_.pointLights[j].pad0
			});
		i++;
		lpCBufferMng.SetPixelCbuffer(CBUFFER_POINTLIGHT, i,
			{
			light_.pointLights[j].color.x ,
			light_.pointLights[j].color.y,
			light_.pointLights[j].color.z,

			light_.pointLights[j].range
			});
		i++;
	}

	i = 0;
	for (int j = 0; j < SPOT_LIGHT_NUM; j++)
	{
		light_.spotLights[j].Position = {0,0,0};

		light_.spotLights[j].Color.x = (j == 0) ? PLAYER_LIGHT_COLOR : ENEMY_LIGHT_COLOR;
		light_.spotLights[j].Color.y = (j == 0) ? PLAYER_LIGHT_COLOR : 0.0f;
		light_.spotLights[j].Color.z = (j == 0) ? PLAYER_LIGHT_COLOR : 0.0f;

		light_.spotLights[j].Direction.x = 0.0f;
		light_.spotLights[j].Direction.y = 0.0f;
		light_.spotLights[j].Direction.z = 0.0f;

		light_.spotLights[j].Direction.Normalize();
		light_.spotLights[j].Range = (j == 0) ? PLAYER_LIGHT_RANGE : ENEMY_LIGHT_RANGE;
		light_.spotLights[j].Angle = (j == 0) ? Deg2RadF(PLAYER_LIGHT_ANGLE): Deg2RadF(ENEMY_LIGHT_ANGLE);
		
		lpCBufferMng.SetPixelCbuffer(CBUFFER_SPOTLIGHT, i,
			{
			light_.spotLights[j].Position.x ,
			light_.spotLights[j].Position.y,
			light_.spotLights[j].Position.z,

			light_.spotLights[j].pad0
			});
		i++;
		lpCBufferMng.SetPixelCbuffer(CBUFFER_SPOTLIGHT, i,
			{
			light_.spotLights[j].Color.x ,
			light_.spotLights[j].Color.y,
			light_.spotLights[j].Color.z,

			light_.spotLights[j].Range
			});
		i++;
		lpCBufferMng.SetPixelCbuffer(CBUFFER_SPOTLIGHT, i,
			{
			light_.spotLights[j].Direction.x ,
			light_.spotLights[j].Direction.y,
			light_.spotLights[j].Direction.z,

			light_.spotLights[j].Angle
			});
		i++;
	}

	light_.ambientLight = lpCBufferMng.GetALLambientLight();
	lpCBufferMng.SetPixelCbuffer(CBUFFER_AMBIENTLIGHT, 0,
		{
		light_.ambientLight.x,
		light_.ambientLight.y,
		light_.ambientLight.z,
		light_.specPow });
}

void GameCBuffer::Update(Vector3 cameraPos,Vector3 cameraTargetPos, Vector3 enemyEyePos, Vector3 enemyDir, Vector3 enemyTargetPos)
{

	lpCBufferMng.Update();
	//ポイントライトの点滅
	if (std::rand() % POINT_BETWEEN == 0)
	{
		light_.pointLights[0].color = { POINT_COLOR_ZERO,POINT_COLOR_ZERO,POINT_COLOR_ZERO };
	}
	else
	{
		light_.pointLights[0].color = { POINT_COLOR_MAX0,POINT_COLOR_MAX0,POINT_COLOR_MAX0 };
	}
	int i = 0;
	for (int j = 0; j < POINT_LIGHT_NUM; j++)
	{

		i++;
		lpCBufferMng.SetPixelCbuffer(CBUFFER_POINTLIGHT, i,
			{
			light_.pointLights[j].color.x ,
			light_.pointLights[j].color.y,
			light_.pointLights[j].color.z,

			light_.pointLights[j].range
			});
		i++;
	}

	light_.spotLights[PLAYER_LIGHT].Position = cameraPos;
	light_.spotLights[PLAYER_LIGHT].Direction = cameraTargetPos - cameraPos;

	light_.spotLights[ENEMY_LIGHT].Position = enemyEyePos;
	light_.spotLights[ENEMY_LIGHT].Position.y = enemyEyePos.y + ENEMY_LIGHT_POSITION_OFFSET_Y;
	light_.spotLights[ENEMY_LIGHT].Direction = enemyDir;
	light_.spotLights[ENEMY_LIGHT].Direction.y = enemyDir.y- ENEMY_LIGHT_DIRECTION_OFFSET_Y;
	light_.spotLights[ENEMY_LIGHT].Range = ENEMY_LIGHT_RANGE;

	float length;
	bool flag = false;
	for (int i = 0; i < markerPos_.size(); i++)
	{
		if (static_cast<int>(cameraPos.y) == static_cast<int>(markerPos_[i].y))
		{
			length = (cameraPos - markerPos_[i]).Magnitude();
			//距離を取り座標に近づいたらフラグをオンにする
			if (length < PLAYER_WEAK_LIGHT_LENGTH)
			{
				flag = true;
			}
		}
	}
	if (flag)
	{
		//オンだったら
		light_.spotLights[PLAYER_LIGHT].Color = { SPOT_COLOR_MIN_0,SPOT_COLOR_MIN_0,SPOT_COLOR_MIN_0 };
	}
	else
	{
		//オフだったら
		light_.spotLights[PLAYER_LIGHT].Color = { SPOT_COLOR_MAX_0,SPOT_COLOR_MAX_0,SPOT_COLOR_MAX_0 };
	}

	i = 0;
	for (int j = 0; j < SPOT_LIGHT_NUM; j++)
	{
		lpCBufferMng.SetPixelCbuffer(CBUFFER_SPOTLIGHT,i,
			{
			light_.spotLights[j].Position.x ,
			light_.spotLights[j].Position.y,
			light_.spotLights[j].Position.z,

			light_.spotLights[j].pad0
			});
		i++;
		lpCBufferMng.SetPixelCbuffer(CBUFFER_SPOTLIGHT, i,
			{
			light_.spotLights[j].Color.x ,
			light_.spotLights[j].Color.y,
			light_.spotLights[j].Color.z,

			light_.spotLights[j].Range
			});
		i++;
		light_.spotLights[j].Direction.Normalize();
		lpCBufferMng.SetPixelCbuffer(CBUFFER_SPOTLIGHT, i,
			{
			light_.spotLights[j].Direction.x ,
			light_.spotLights[j].Direction.y,
			light_.spotLights[j].Direction.z,

			light_.spotLights[j].Angle
			});

		i++;
	}
	light_.ambientLight = lpCBufferMng.GetALLambientLight();
	lpCBufferMng.SetPixelCbuffer(CBUFFER_AMBIENTLIGHT, 0,
		{
		light_.ambientLight.x,
		light_.ambientLight.y,
		light_.ambientLight.z,
		light_.specPow });

	
#ifdef _DEBUG

#endif	//_DEBUG
}

