#pragma once
#include "BaseScene.h"
#include "../tmx/TmxObj.h"

enum class CntType;
class EndingUI;

class EndingScene :
    public BaseScene
{
public:
    EndingScene();
    EndingScene(CntType type);
    ~EndingScene();

    // 初期化処理
    void Init() override;

    // 更新ステップ
    UniqueScene Update(UniqueScene scene) override;

    // 描画処理
    void DrawScreen(void) override;

    // 解放処理
    void Release(void) override;

    //シーンのIDを返す
    SceneID GetSceneID(void) override { return SceneID::Ending; };


private:
    void UpdateSE();
    //シーン切り替え関数
    UniqueScene UpdateScene(UniqueScene& scene);

    ////カメラ情報
    //std::unique_ptr<Camera> camera_;

    Vector3 pos_;
    std::string modleStr_;

    std::unique_ptr<EndingUI> endingUI_;
    CntType cntType_;
    std::unique_ptr<Controller> controller_;
};

