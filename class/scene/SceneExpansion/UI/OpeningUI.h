#pragma once
#include "UI.h"

enum class OpeningColUI
{
    Non,
    First,//一つ目の文
    Second,//二つ目の文
    Third,//三つ目の文
    Forth,//四つ目の文
    Five,//五つ目の文
    Six,//六つ目の文
    Seven,//七つ目の文
    Max
};

class Controller;
enum class ControllerType;

class OpeningUI :
    public UI
{
public:
    OpeningUI();
    OpeningUI(CntType cntType);
    ~OpeningUI();
    void Init()override;
    void Update()override;
    void Draw()override;
    //オープニングが終わったか
    bool GetIsEndFlg();
    int Getprogress();
private:
    //UIMapの初期化
    void InitUIMap();
    //コントローラ情報
    std::unique_ptr<Controller> controller_;
    CntType cntType_;
    //UIMap
    std::map<OpeningColUI, UIObj<OpeningColUI>> openingUIMap_;

    double time_;//経過時間
    int progress_;//進捗度
    bool isEndFlg;//表示時間終了フラグ
};

