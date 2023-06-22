#pragma once
#include<memory>
#include "BaseScene.h"
#include "../tmx/TmxObj.h"

class OptionUI;

class Camera;
class GameCBuffer;
class OptionScene :
    public BaseScene
{
public:
    OptionScene(UniqueScene ownScene,SceneID beforeID);
    ~OptionScene();

    // 初期化処理
    void Init(void) override;

    // 更新ステップ
    UniqueScene Update(UniqueScene scene) override;

    // 描画処理
    void DrawScreen(void) override;

    // 解放処理
    void Release(void) override;

    //シーンのIDを返す
    SceneID GetSceneID(void) override { return SceneID::Option; };
private:
    //シーン切り替え関数
    UniqueScene UpdateScene(UniqueScene& scene);

    UniqueScene beforeScene_;    //前のシーン(主にゲームシーン)
   
    SceneID beforeSceneID_;      //前のシーンID
    //UI
    std::unique_ptr<OptionUI> optionUI_;
    //バッファ
    std::unique_ptr<GameCBuffer> cBuffer_;
    //カメラ情報
    std::unique_ptr<Camera> camera_;
};