#pragma once
#include<memory>

class Controller;
enum class ControllerType;

class Stamina
{
public:
    Stamina(ControllerType type);
    ~Stamina();

    //初期化
    void Init(void);
    //破棄
    void Release(void);
    //係数の計算と経過時間処理
    float Update(bool isCamera);
    //描画
    void Draw(void);

private:
    //移動係数とダッシュ時間の計算
    void CalcCoefficientAndTime(void);

    //スタミナゲージのアルファ値処理
    void CalcStaminaGaugeAlpha(bool transFlag);

    //スタミナゲージの色変更処理
    void ChangeStaminaGaugeColor(void);

    //コントローラ情報
    std::unique_ptr<Controller> controller_;

    //移動係数
    float moveCoefficient_;

    //ダッシュ時間
    double dashTime_;

    //アルファ値
    int gaugeAlpha_;

    //ゲージの色
    int gaugeColor;

    //ダッシュの切り替えフラグ
    bool dashFlag_;
    bool isCamera_;
};

