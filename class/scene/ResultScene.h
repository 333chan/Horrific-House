#pragma once
#include "BaseScene.h"
#include "../tmx/TmxObj.h"

class Controller;
class ResultUI;
class Camera;
class ResultCBuffer;
class OriginalShader;
enum class ControllerType;

class ResultScene :
    public BaseScene
{
public:
    ResultScene(ControllerType type);
    ~ResultScene();

    // 初期化
    void Init(void) override;

    // 更新
    UniqueScene Update(UniqueScene scene) override;

    // 描画
    void DrawScreen(void) override;

    //破棄
    void Release(void) override;

    //シーンのIDを返す
    SceneID GetSceneID(void) override { return SceneID::Result; };
private:

    //キューブマップの生成
    void CreateCubeMap(void);
    //シーン切り替え関数
    UniqueScene UpdateScene(UniqueScene& scene);

    //コントローラ情報
    std::unique_ptr<Controller> controller_;
    //コントローラタイプ
    ControllerType contorollerType_;

    //カメラ情報
    std::unique_ptr<Camera> camera_;

    //シェーダ
    std::unique_ptr<OriginalShader> origShader_;

    //UI
    std::unique_ptr<ResultUI> resultUI_;

    //定数バッファ
    std::unique_ptr<ResultCBuffer> cBuffer_;

    //定数バッファで使用するカウント
    float stepCount_;

    //キューブマップハンドル
    int cubeTexture_;

    //スカイドームの回転
    float skyRotate_;
};
