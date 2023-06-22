#pragma once
#include "UI.h"

enum class SelectColUI
{
	Non,
	Title,
	Key,
	Pad,
	Ar,
	Decide,
	Return,
	Mouse,
	Operation,
	Max
};

class Controller;
enum class ControllerType;
class SelectUI :
    public UI
{
public:
    SelectUI();
    ~SelectUI();
    void Init()override;
    void Update()override;
    void Draw()override;
	//当たったUI
	SelectColUI GetHitUICol();
	//選択したコントローラータイプ取得
	CntType GetCntType();
private:
	//UIMapの初期化
	void InitUIMap();
	//クリックして当たったの
	void PutClickHit();
	//コントローラ情報
	std::unique_ptr<Controller> controller_;
	//UIMap
	std::map<SelectColUI, UISelectObj<SelectColUI>> selectUIMap_;
	//決定当たったUI
	SelectColUI decideHitCol_;
	//選択したコントローラータイプ
	CntType cntType_;
};

