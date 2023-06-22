#pragma once
#include "BaseScene.h"

class ExitUI;
class ExitScene :
    public BaseScene
{
public:
    ExitScene();
    ExitScene(SceneID sceneID);
    ExitScene(SceneID sceneID, UniqueScene ownScene, SceneID beforeID);

    ~ExitScene();

    // 初期化処理
    void Init() override;

    // 更新ステップ
    UniqueScene Update(UniqueScene scene) override;

    // 描画処理
    void DrawScreen(void) override;

    // 解放処理
    void Release(void) override;

    //シーンのIDを返す
    SceneID GetSceneID(void) override { return SceneID::Exit; };

private:
    
    //シーン切り替え関数
    UniqueScene UpdateScene(UniqueScene& scene);
    SceneID sceneID_;           //一個前のシーン
    UniqueScene beforeScene_;    //オプションシーンの前のシーン
    SceneID beforeSceneID_;      //オプションシーンの前のシーンID
    //UI
    std::unique_ptr<ExitUI>exitUI_;
};

