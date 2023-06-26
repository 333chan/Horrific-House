#pragma once
#include "UI.h"

enum class SceneID;
enum class ExitColUI
{
	Non,
	Title,
	Yes,
	YesOption,
	No,
	NoOption,
	Max
};

class Controller;
enum class ControllerType;

class ExitUI :
    public UI
{
public:	
	ExitUI();
	ExitUI(SceneID sceneID);
	~ExitUI();
	void Init()override;
	void Update()override;
	void Draw()override;
	//当たったUI取得
	ExitColUI GetHitUICol();

private:
	//UIMapの初期化
	void InitUIMap();
	//クリックして当たったの
	void PutClickHit();
	//コントローラ情報
	std::unique_ptr<Controller> controller_;
	//UIMap
	std::map<ExitColUI, UIObj<ExitColUI>> exitUIMap_;
	//決定当たったUI
	ExitColUI decideHitCol_;
	//一個前のシーン
	SceneID sceneID_;
};

