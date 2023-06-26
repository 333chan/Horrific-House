#pragma once
#include<memory>
#include "BaseScene.h"

class ConnectedUI;

class ConnectedScene :
    public BaseScene
{
public:
    ConnectedScene();
    ~ConnectedScene();

    // 初期化処理
    void Init() override;

    // 更新ステップ
    UniqueScene Update(UniqueScene scene) override;

    // 描画処理
    void DrawScreen(void) override;

    // 解放処理
    void Release(void) override;

    //シーンのIDを返す
    SceneID GetSceneID(void) override { return SceneID::Select; };

private:
    //シーン切り替え関数
    UniqueScene UpdateScene(UniqueScene& scene);
    //UI
    std::unique_ptr<ConnectedUI> connectedUI_;

};
