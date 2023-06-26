#pragma once
#include "BaseScene.h"


class PreTitleUI;

class PreTitleScene :
    public BaseScene
{
public:
    PreTitleScene();
    ~PreTitleScene();

    // 初期化処理
    void Init() override;

    // 更新ステップ
    UniqueScene Update(UniqueScene scene) override;

    // 描画処理
    void DrawScreen(void) override;

    // 解放処理
    void Release(void) override;

    //シーンのIDを返す
    SceneID GetSceneID(void) override { return SceneID::PreTitle; };

private:
    //UI
    std::unique_ptr<PreTitleUI> preTitleUI_;
};

