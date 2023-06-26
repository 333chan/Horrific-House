#pragma once
#include<vector>
#include<chrono>
#include<string>
#include "BaseScene.h"
#include "../common/Geometry.h"
#include "../common/Light.h"

class Camera;
class Controller;
class Stage;
class Player;
class Enemy;

class CBufferMng;
class SocketServer;
class Goal;
class OriginalShader;
class GameCBuffer;
class GameUI;
enum class CntType;
class EffekseerEffectHandle;
class Effects;

class GameScene :
    public BaseScene
{
public:
    GameScene();
	GameScene(CntType type);
	~GameScene();

    // 初期化処理
    void Init(void) override;

    // 更新ステップ
    UniqueScene Update(UniqueScene scene) override;

    // 描画処理
    void DrawScreen(void) override;

    // 解放処理
    void Release(void) override;

    //シーンのIDを返す
    SceneID GetSceneID(void) override { return SceneID::Game; };

private:
    //コントローラ情報
    std::unique_ptr<Controller> controller_;

    //ステージ情報
    std::unique_ptr<Stage> stage_;

    //カメラ情報
    std::unique_ptr<Camera> camera_;

    //プレイヤー情報
    std::unique_ptr<Player> player_;

    //敵情報
    std::unique_ptr<Enemy> enemy_;

    //ゴール情報
    std::unique_ptr<Goal> goal_;
    //定数バッファ
    std::unique_ptr<GameCBuffer> cBuffer_;
    //描画用シェーダー
    std::unique_ptr<OriginalShader> origShader_;
    //深度シェーダー
	std::unique_ptr<OriginalShader> depthShader_;
    //UI
    std::unique_ptr<GameUI> gameUI_;
    //スクリーン
    int screenCameraID_;
    int GaussScreenCameraID_;
	int DepthMapTextureID_;
   
    //インベントリの背景ハンドル
    int inventoryBackGroundHandle_;

    //ポストエフェクト用
    std::unique_ptr<OriginalShader> postEffect_;
    int psPostEffect_;
    float postEffectShade_;
    CntType cntType_;
    //デフォルト座標
    Vector3 defPos_;
    //デフォルトターゲット座標
    Vector3 defTargetPos_;

    //--Effekseer--
    std::vector<EffekseerEffectHandle> effHandle_;//エフェクトリソース
    std::vector<Effects> effects_;                //エフェクト管理用

    //void AddEffect(std::string effecName);
    //----------

    //エフェクト
    Effect dust_;
};
