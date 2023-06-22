#pragma once
#include <string>
#include "BaseScene.h"
#include "../common/Geometry.h"

class Camera;
class OriginalShader;
class TitleCBuffer;
class TitleUI;

class TitleScene :
    public BaseScene
{
public:
    TitleScene();
    ~TitleScene();

    // 初期化処理
    void Init() override;

    // 更新ステップ
    UniqueScene Update(UniqueScene scene) override;

    // 描画処理
    void DrawScreen(void) override;

    // 解放処理
    void Release(void) override;

    //シーンのIDを返す
    SceneID GetSceneID(void) override { return SceneID::Title; };

private:
    //シーン切り替え関数
    UniqueScene UpdateScene(UniqueScene& scene);

    //カメラ情報
    std::unique_ptr<Camera> camera_;
    //モデルの座標
    Vector3 pos_;
    //モデルの文字列
    std::string modleStr_;
    //シェーダー描画用
    std::unique_ptr<OriginalShader> origShader_;
    //タイトルの定数バッファ
    std::unique_ptr<TitleCBuffer> cBuffer_;
    //タイトルのUI
    std::unique_ptr<TitleUI> titleUI_;

};
