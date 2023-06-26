#pragma once
#include<chrono>
#include<map>
#include <DxLib.h>
#include"EffekseerForDXLib.h"
#include"../common/Geometry.h"
#include "BaseScene.h"

#define lpSceneMng SceneManager::GetInstance()

class MouseIcon;
class Fps;

class SceneManager
{
public:

	static SceneManager& GetInstance()
	{
		static SceneManager s_Instance;
		return s_Instance;
	}

	//初期化
	void Init(void);
	void Init3D(void);
	//更新
	void Run(void);

	//画面サイズ取得
	Vector2Int GetScreenSize(void);

	//デルタタイム取得
	double GetDeltaTime(void);

	//定数バッファにデータを入れる処理
	void SetBrightnessScrren(int brightnessScrren);
	int GetBrightnessScrren();
	void AddInventoryCnt();
	int GetInventoryCnt();
private:
	//システム初期化処理
	bool SysInit(void);
	SceneManager();
	~SceneManager();

	void Update(void);
	void Draw(void);

	void UpdateMouse();
	//シーン情報
	UniqueScene scene_;
	int sceneGraphFilter_;
	//経過時間
	std::chrono::system_clock::time_point tickCount_;

	//デルタタイム
	double deltaTime_;

	int brightnessScrren_;
	//std::unique_ptr<MouseIcon> mouseIcon_;
	int inventoryCnt_;

	std::unique_ptr<Fps> fps_;
};
