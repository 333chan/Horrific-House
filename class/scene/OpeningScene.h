#pragma once
#include "BaseScene.h"

enum class CntType;
class OpeningUI;

class OpeningScene : 
	public BaseScene
{
public:
    OpeningScene();
    OpeningScene(CntType type);
    ~OpeningScene();

    // 初期化処理
    void Init() override;

    // 更新ステップ
    UniqueScene Update(UniqueScene scene) override;

    // 描画処理
    void DrawScreen(void) override;

    // 解放処理
    void Release(void) override;

    //シーンのIDを返す
    SceneID GetSceneID(void) override { return SceneID::Opening; };

   
private:
    //SE更新
    void UpdateSE();
    //UI
    std::unique_ptr<OpeningUI> openingUI_;
    //選択したコントローラータイプ
    CntType cntType_;
};

