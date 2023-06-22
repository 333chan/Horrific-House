#pragma once
#include "Gimmick.h"
class AnimationController;

class Door :
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
    //Door(Vector3 pos,Vector3 angle, std::string name, int num, int collNum);
    Door(std::string name, Vector3 pos, Vector3 angle, Vector3 scale, int num);
    ~Door();

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
    //アニメーション
    std::unique_ptr<AnimationController> animController_;
    std::unique_ptr<OriginalShader> origShader_;
    //変数初期化
    void VariableInit(void);
    //アニメーション処理
    void Animation(void);
    //ライティング
    int psLight_;
    int vsLight_;
    //アニメーションフラグ
    bool animFlg_;

};

