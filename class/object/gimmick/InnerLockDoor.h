#pragma once
#include "Gimmick.h"

class AnimationController;

class InnerLockDoor :
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
    InnerLockDoor(std::string name, Vector3 pos, Vector3 angle, Vector3 scale, int num ,float normal);
    ~InnerLockDoor();

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
    //変数初期化
    void VariableInit(void);
    //アニメーション処理
    void Animation(void);

    //描画時間計算
    void DrawTimeCount();

    //UI描画
    void DrawUI();

    //シェーダー
    std::unique_ptr<OriginalShader> origShader_;
    //ライティング
    int psLight_;
    int vsLight_;
    //指定した方向から扉を開けるための法線
    float normal_;

    //経過時間
    double delta_;

    //UI表示フラグ
    bool uiFlg_;

    //画像サイズの半分の値
    Vector2Int graphSize_;

    //描画位置
    Vector2Int drawPos;
    std::string uiKey_;
    //アニメーションフラグ
    bool animFlg_;

    //初回判定用フラグ
    bool firstTimeFlg_;
};

