#pragma once
#include<DxLib.h>
#include<memory>
#include"../Common/Geometry.h"
#include"../common/Quaternion.h"

//カメラの高さ(仮)
constexpr float CAMERA_HEIGHT = 1250.0f;

enum class LiftType
{
	UP,
	DOWN,
	MAX,
};

class Camera
{
public:
	Camera();
	Camera(Vector3 cameraAngle);
	~Camera();

	//初期化
	void Init(void);

	//更新
	void Update(bool dashFlag);
	void Update(Vector3 targetPos);//ゲームオーバー時の演出用
	//追従カメラ(初期化されるのでオブジェクト描画前にする必要がある)
	void SetBeforeDraw(Vector3 playerpos);
	void SetBeforeDraw_dead(Vector3 playerpos,Vector3 targetpos);//targetposを見つづける

	//描画
	void Draw(void);
	void SetSensitivity(float sens);
	//カメラ座標を返す
	Vector3 GetPos();

	//注視点を返す
	Vector3 GetTargetPos();

	//角度を返す
	Vector3 GetAngles();

	//画面揺れ
	float SwingWidth(void);

private:
	void FallHead(void);//おられた首が落ちる表現
	bool BreakHead(void);//首をおる表現
	bool WaitTime(double time);//指定された時間待つ
	//カメラの位置
	Vector3 pos_;
	Vector3 offset_;//カメラのオフセット（ゲームオーバーでカメラ上にあげてる）

	//カメラ角度(rad)
	Vector3 angle_;
	float defaultangle_y;
	float roll_;
	float sensitivity_;
	//角度(クォータニオン)
	Quaternion quaRot_;

	//注視点
	Vector3 targetPos_;
	Vector3 t_offset_;//注視点を敵の顔が見えるくらいに調整するよう（ゲームオーバー）

	Vector2Int mousePos_;
	Vector3 padPos_;

	int sinCnt_;
	double fallCnt_;//落下の経過時間
	bool liftFlg_;//上下移動フラグ
	double waitTime_;//演出時の待ち時間
	double fallAccel_ = 0.0;

	float sinCount_;
};
