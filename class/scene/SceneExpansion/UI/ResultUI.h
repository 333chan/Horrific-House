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
    //��������UI
    ResultColUI GetHitUICol();
    //�R���g���[���[�^�C�v���擾
    ControllerType GetControllerType();
private:
    //UIMap�̏�����
    void InitUIMap();
    //�N���b�N���ē���������
    void PutClickHit();

    void CreditProcess(CreditData& creditData,Vector2Int pos);

    //�R���g���[�����
    std::unique_ptr<Controller> controller_;
    //UIMap
    std::map<ResultColUI, UIObj<ResultColUI>> resultUIMap_;
    //���蓖������UI
    ResultColUI decideHitCol_;

    //Vector2Int createrNamePos_;
    Vector2Int roleNamePos_;

    CreditData creator_;
    CreditData role_;

    RoleType roleType_;
    double stepTime_;

    bool lastFlag_;
};

