#pragma once
#include <vector>
#include "CBuffer.h"

constexpr int DIRECTION_LIGHT_NUM = 3;  // ディレクションライトの数
constexpr int POINT_LIGHT_NUM = 3;   // ポイントライトの数
constexpr int SPOT_LIGHT_NUM = 2;   // ポイントライトの数

struct GameLight
{
	// ディレクションライト用のメンバ
	DirectionlLight directionLights[DIRECTION_LIGHT_NUM];
	PointLight       pointLights[POINT_LIGHT_NUM];
	SpotLight        spotLights[SPOT_LIGHT_NUM];

	Vector3 ambientLight;   // アンビエントライト
	float specPow;
};

class GameCBuffer :
    public CBuffer
{

public:
	GameCBuffer();
	GameCBuffer(Vector3 defPos, Vector3 defTargetPos);
	~GameCBuffer();
	void Init(void);
	void Update(Vector3 cameraPos, Vector3 cameraTargetPos,Vector3 enemyPos,Vector3 enemyDir, Vector3 enemyTargetPos);

private:
    GameLight light_;
	double FlashingCnt_;
	float camera_[16];
	float Pcamera_[16];

	Vector3 defPos_;
	Vector3 defTargetPos_;
	std::vector<Vector3> markerPos_;
};

