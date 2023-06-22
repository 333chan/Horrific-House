#pragma once
#include "Item.h"

class OriginalShader;

class Memo :
    public Item
{
public:
    Memo(std::string name, Vector3 bPos, Vector3 aPos, Vector3 angle, Vector3 scale, ItemID memoID, int num);
    ~Memo();

    //初期化
    void Init(void)override;

    //更新
    void Update(void)override;

    //描画
    void Draw(void)override;
    void Draw(bool cameraflag);
    //開放
    void Release(void)override;

    //アイテムIDを返す
    ItemID GetItemID(void)override { return itemId_; };
private:
    //移動処理
    void Move(void);
    //シェーダ
    std::unique_ptr<OriginalShader> origShader_;
    //移動状態
    ItemMove moveState_;
    //アニメーション状態
    ItemAnim animState_;
};

