#pragma once
#include "UI.h"
#include "../../../common/Geometry.h"
#include "../../../tmx/TmxObj.h"

enum class GameOverColUI
{
	Non,
	GameOver,
	ToTitle,
	Restart,
	Max
};

class Controller;
enum class ControllerType;

class GameOverUI :
    public UI
{
public:
	GameOverUI();
	~GameOverUI();
	void Init()override;
	void Update()override;
	void Draw()override;
	//当たったUI
	GameOverColUI GetHitUICol();

private:
	//UIMapの初期化
	void InitUIMap();
	//クリックして当たったの
	void PutClickHit();

	//コントローラ情報
	std::unique_ptr<Controller> controller_;
	//UIMap
	std::map<GameOverColUI, UIObj<GameOverColUI>> gameoverUIMap_;
	//決定当たったUI
	GameOverColUI decideHitCol_;

};

