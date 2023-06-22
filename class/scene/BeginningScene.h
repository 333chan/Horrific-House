#pragma once
#include <map>
#include "BaseScene.h"
enum class beginningType
{
    Non,
    Logo,
    Warning,
};


class BeginningUI;


class BeginningScene :
    public BaseScene
{
public:
    BeginningScene();
    BeginningScene(beginningType type);
    ~BeginningScene();

    // 初期化処理
    void Init() override;

    // 更新ステップ
    UniqueScene Update(UniqueScene scene) override;

    // 描画処理
    void DrawScreen(void) override;

    // 解放処理
    void Release(void) override;

    //シーンのIDを返す
    SceneID GetSceneID(void) override { return SceneID::Beginning; };

private:
    //描画関数ポインタに代入する関数
    void UpdateLogo();
    void UpdateWarning();
    //描画関数ポインタに代入する関数
    void DrawLogo();         
    void DrawWarning();           
    //次のシーン
    beginningType afterScene_;

    //更新関数ポインタ
    void (BeginningScene::* updateFuncPtr_)();
    //描画関数ポインタ
    void (BeginningScene::* drawFuncPtr_)();
    //今のタイプと更新、描画関数ポインタのマップ
    std::map<beginningType, std::pair<void (BeginningScene::*)(), void (BeginningScene::*)()>> beginningTypeMap_;
    //経過時間
    double elapsedTime_;
};
