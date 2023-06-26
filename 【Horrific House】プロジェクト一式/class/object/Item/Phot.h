#pragma once
#include "Item.h"

class OriginalShader;

class Phot
    : public Item
{
public:
    Phot(std::string name, Vector3 bPos, Vector3 aPos, Vector3 angle, Vector3 scale, ItemID id, int num);
    ~Phot();

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
    ItemID GetItemID(void)override { return ItemID::Phot; };

private:
    std::unique_ptr<OriginalShader> origShader_;

protected:

};

