#pragma once
#include <vector>
#include "Gimmick.h"

class AnimationController;

//相対位置
enum class RELATIVE_LOC {
    LEFT_UP,
    RIGHT_UP,
    LEFT_DOWN,
    RIGHT_DOWN
};

class Drawer :
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
    Drawer(std::string name, Vector3 pos, Vector3 angle, Vector3 scale, int num);
    ~Drawer();

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

    //モデルのハンドルの文字列を返す
    std::string GetCollModelStr(void)override;

private:
    //アニメーション
    std::unique_ptr<AnimationController> animController_;
    //シェーダー
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

