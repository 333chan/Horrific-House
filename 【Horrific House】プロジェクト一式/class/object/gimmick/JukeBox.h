#pragma once
#include "Gimmick.h"
class JukeBox :
    public Gimmick
{
public:
    JukeBox(std::string name, Vector3 pos, Vector3 angle, Vector3 scale, int num);
    ~JukeBox();
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
    //ライティング
    int psLight_;
    int vsLight_;
};