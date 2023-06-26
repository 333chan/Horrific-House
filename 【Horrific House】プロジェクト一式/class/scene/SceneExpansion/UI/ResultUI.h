#pragma once
#include "UI.h"
#include "../../../input/Controller.h"

enum class ResultColUI
{
    Non,
    Title,
    reTitle,
    Max
};

enum class CreditTargetDir
{
    RIGHT,
    CENTER,
    LEFT,
    MAX
};

enum class RoleType
{
    PLAN,
    PROGRAMMER,
    MAX
};

struct CreditData
{
    Vector2Int pos;
    double stepLerpTime = 0.0;
    double stepStopTime = 0.0;
    CreditTargetDir dir = CreditTargetDir::CENTER;
    bool roleFlag = false;
};

class ResultUI :
    public UI
{
public:
    ResultUI();
    ~ResultUI();
    void Init()override;
    void Update()override;
    void Draw()override;
    //当たったUI
    ResultColUI GetHitUICol();
    //コントローラータイプを取得
    ControllerType GetControllerType();
private:
    //UIMapの初期化
    void InitUIMap();
    //クリックして当たったの
    void PutClickHit();

    void CreditProcess(CreditData& creditData,Vector2Int pos);

    //コントローラ情報
    std::unique_ptr<Controller> controller_;
    //UIMap
    std::map<ResultColUI, UIObj<ResultColUI>> resultUIMap_;
    //決定当たったUI
    ResultColUI decideHitCol_;

    //Vector2Int createrNamePos_;
    Vector2Int roleNamePos_;

    CreditData creator_;
    CreditData role_;

    RoleType roleType_;
    double stepTime_;

    bool lastFlag_;
};

