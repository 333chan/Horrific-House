#pragma once
#include "BaseScene.h"
enum class CntType;
class LoadUI;

class LoadScene :
    public BaseScene
{

public:
    LoadScene();
    LoadScene(CntType type);
    ~LoadScene();
    
    // 初期化処理
    void Init(void) override;

    // 更新ステップ
    UniqueScene Update(UniqueScene scene) override;

    // 描画処理
    void DrawScreen(void) override;

    // 解放処理
    void Release(void) override;

    //シーンのIDを返す
    SceneID GetSceneID(void) override { return SceneID::Load; };

private:
    //UI
    std::unique_ptr<LoadUI> loadUI_;
    //選択したコントローラータイプ
    CntType cntType_;
};

