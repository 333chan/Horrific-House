#pragma once
#include<DxLib.h>
#include<memory>
#include<array>
#include "../common/Geometry.h"
#include "../common/SceneID.h"

class BaseScene;

using UniqueScene = std::unique_ptr<BaseScene>;


//エフェクト構造体
struct Effect
{
	int resource = -1;     //ハンドル
	int playhandle = -1;   //プレイハンドル
	Vector3 pos; //位置
	float angle = 0.0f; //角度
};

class BaseScene
{
public:
	BaseScene();
	virtual ~BaseScene();
	// 初期化処理
	virtual void Init(void) = 0;

	void InitVertext(void);

	// 更新ステップ
	virtual UniqueScene Update(UniqueScene scene) = 0;

	// 描画処理
	virtual void Draw();
	virtual void DrawPostEffect(int ps);
	virtual void DrawPostEffect(int ps,int texture1,int texture2);
	virtual void DrawScreen(void) = 0;

	// 解放処理
	virtual void Release(void) = 0;

	virtual SceneID GetSceneID(void) = 0;
	
private:

protected:

	void PreLoading();

	void Loading();

	void AfterLoading();
	//スクリーン情報
	int screenID_;

	//アルベドレンダーターゲット
	int albedRT_;
	//法線レンダーターゲット
	int normalSpecRT_;
	//ワールド座標レンダーターゲット
	int depthRT_;

	//スクリーンの大きさ
	int screenSizeX_;	//横
	int screenSizeY_;	//縦

	//Primitive2D用の頂点データ
	std::array<VERTEX2DSHADER, 4> verts_;
};