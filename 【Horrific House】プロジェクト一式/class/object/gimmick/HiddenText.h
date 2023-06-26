#pragma once
#include "Gimmick.h"
class HiddenText :
    public Gimmick
{
public:
    /// <summary>
    /// コンストラクタ
    /// </summary>
    /// <param name="初期座標"></param>
    /// <param name="初期角度"></param>
    /// <param name="オブジェクト名"></param>
    /// <param name="複製数"></param>
    /// <param name="法線の向き"></param>
    HiddenText(std::string name, Vector3 pos, Vector3 angle, Vector3 scale, int num);
    ~HiddenText();

    //初期化
    void Init(void)override;

    //更新
    void Update(void)override;

    //描画
    void Draw(void)override;

    //開放
    void Release(void)override;

    //モデルの個数を返す
    int GetModelNum(void)override;

    //モデルハンドルの文字列を返す
    std::string GetCollModelStr(void)override;
private:
    //シェーダー
    std::unique_ptr<OriginalShader> origShader_;
    //変数初期化
    void VariableInit(void);

};

