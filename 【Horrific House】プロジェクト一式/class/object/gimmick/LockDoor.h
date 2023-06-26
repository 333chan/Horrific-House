#pragma once
#include "Gimmick.h"

class AnimationController;
class LockDoor :
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
    /// <param name="解除するために必要なアイテム名"></param>
    //LockDoor(Vector3 pos, Vector3 angle, std::string name, int num,int collNum,ItemID itemID);
    LockDoor(std::string name, Vector3 pos, Vector3 angle, Vector3 scale, int num,ItemID itemID);
    ~LockDoor();

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

    //UI描画時間計算
    void DrawTimeCount(void);

    void SetGraphSize(void);

    //UI描画
    void DrawUI(void);

    //UI画像識別用
    std::string uiKey_;

    //画像サイズの半分の値
    Vector2Int graphSize_;

    //描画位置
    Vector2Int drawPos;

    //ライティング
    int psLight_;
    int vsLight_;

    //経過時間
    double delta_;

    //初回確認用フラグ
    bool firstTimeFlg_;

    //UI表示判定用フラグ
    bool uiFlg_;

    //アニメーション再生フラグ
    bool animFlg_ = false;

};

