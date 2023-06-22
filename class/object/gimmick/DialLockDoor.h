#pragma once
#include "Gimmick.h"
class AnimationController;
class DialLockDoor :
    public Gimmick
{
public:
    /// <summary>
    /// 
    /// </summary>
    /// <param name="初期座標"></param>
    /// <param name="初期角度"></param>
    /// <param name="オブジェクト名"></param>
    /// <param name="複製数"></param>
    /// <param name="解除番号"></param>
    DialLockDoor(std::string name, Vector3 pos, Vector3 angle, Vector3 scale, int num,std::string unlockNum);
    ~DialLockDoor();

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

    //シェーダー
    std::unique_ptr<OriginalShader> origShader_;

    //変数初期化
    void VariableInit(void);

    //アニメーション処理
    void Animation(void);

    //UI表示時間処理
    void DrawTimeCount(void);

    //UI描画
    void DrawUI(void);

    //画像サイズの半分の値
    Vector2Int graphSize_;

    //描画位置
    Vector2Int drawPos;

    //ライティング
    int psLight_;
    int vsLight_;

    //経過時間
    double delta_;

    //アニメーションフラグ
    bool animFlg_;

    //UI表示フラグ
    bool uiFlg_;

    //初回判定用フラグ
    bool firstTimeFlg_;
};

