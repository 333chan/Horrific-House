#pragma once
#include<memory>
#include"BaseScene.h"

//class Stage;
class GameScene;
class SelectUI;

class SelectScene :
    public BaseScene
{
public:
    SelectScene();
    ~SelectScene();

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
    std::unique_ptr<SelectUI> selectUI_;

   

};
