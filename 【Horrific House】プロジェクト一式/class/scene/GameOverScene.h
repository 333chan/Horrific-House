#pragma once
#include<memory>
#include "BaseScene.h"
#include "../tmx/TmxObj.h"


class Controller;
class Camera;
class Enemy;
class OriginalShader;
enum class ControllerType;
class GameOverCBuffer;
class GameOverUI;
class AnimationController;
enum class CntType;
class GameOverScene :
    public BaseScene
{
public:
    GameOverScene(ControllerType type,Vector3 playerPos,Vector3 enemyPos,Vector3 cameraAngle, CntType cntType);
    ~GameOverScene();

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

    //UniqueScene beforeScene_;    //前のシーン(主にゲームシーン)
    UniqueScene returningScene_; //シーンを切り替える時用
   // SceneID beforeSceneID_;      //前のシーンID

    //コントローラ情報
    std::unique_ptr<Controller> controller_;
    //敵情報
    std::unique_ptr<Enemy> enemy_;
    //アニメーションコントローラ
    std::unique_ptr<AnimationController> animController_;

    //ファーストはtmxの文字列セカンドは処理のポインター
    std::map<std::string, void (GameOverScene::*)()> selectMap_;//処理まとめリスト
    //std::map<std::string, std::string>* listToDraw_;        //描画するリストのポインタ
    // 
    //ファーストはtmxの文字列セカンドは画像の文字列
    std::map<std::string, std::string> displayMap_;        //描画だけリスト
    std::map<std::string, std::string> toTitleList_;    //タイトルに行くリスト
    std::map<std::string, std::string> reStartList_;    //リスタートリスト
    std::map<std::string, std::string> supportMap_;

    TmxObj tmxObj_;     //衝突判定用tmx
    std::string selection_; //今選択しているものを格納する場所

    std::pair<std::string, std::string> OldhitMouse_;       //今マウスが当たっているものを格納する場所
    std::pair<std::string, std::string> hitMouse_;       //今マウスが当たっているものを格納する場所

    double Cnt_;       //演出のカウント
    int Volume_;    //音量
    int brightness_;//明るさ

    //ーーーーゲームオーバー時の演出関連ーーーー
    std::unique_ptr<Camera> camera_;
    bool directionEndflg_;//演出終了フラグ
    Vector3 playerPos_;//プレイヤーの座標
    Vector3 enemyPos_;//敵本体の座標
    Vector3 ghostPos_;//演出用の敵の座標
    Vector3 dirCameraPos_;//演出用カメラの座標
    Vector3 dirCameraTargetPos_;//演出用カメラの注視点座標
    Vector3 dirCameraPosOffset_;//オフセット
    bool flag_;

    std::unique_ptr<GameOverCBuffer> cBuffer_;
    std::unique_ptr<GameOverUI> gameOverUI_;
    std::unique_ptr<OriginalShader> origShader_;//ステージ描画用
    std::unique_ptr<OriginalShader> origShader2_;//ポストエフェクト描画用

    int renderTarget_;        //レンダリングターゲット
    int renderTarget2_;        //レンダリングターゲット
    int effectRenderTarget_;
   
    void SetRenderTarget(int screenID);
    //シーン切り替え関数
    UniqueScene UpdateScene(UniqueScene& scene);
    //演出の更新
    void UpdateDirection();
    //演出の描画
    void DrawDirection();
    //指定した時間(秒)待つ
    bool WaitCnt(double waitCnt);
    //ポストエフェクト用
    std::unique_ptr<OriginalShader> postEffect_;
    int psPostEffect_;
    float postEffectShade_;

    CntType cntType_;
    
};

