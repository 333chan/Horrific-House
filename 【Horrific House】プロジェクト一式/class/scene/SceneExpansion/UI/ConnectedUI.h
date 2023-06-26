#pragma once
#include "UI.h"

enum class ConnectedColUI
{
	Non,
	Title,
	Text0,
	Text1,
	Text2,
	Image0,
	Image1,
	Image2,
	And,
	Cancel,
	Tab,
	Max
};
class Controller;
enum class ControllerType;
class ConnectedUI :
    public UI
{
public:
	ConnectedUI();
	~ConnectedUI();
	void Init()override;
	void Update()override;
	void Draw()override;
	//当たったUI
	ConnectedColUI GetHitUICol();
	//タイプの
	CntType GetCntType();
private:
	//UIMapの初期化
	void InitUIMap();
	//クリックして当たったの
	void PutClickHit();
	//コントローラ情報
	std::unique_ptr<Controller> controller_;
	//UIMap
	std::map<ConnectedColUI, UIObj<ConnectedColUI>> connetctedUIMap_;
	//決定当たったUI
	ConnectedColUI decideHitCol_;
	//選択したコントローラータイプ
	CntType cntType_;
	//IPアドレス表示用のフォント
	int font_;
};
