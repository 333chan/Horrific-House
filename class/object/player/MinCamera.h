#pragma once
#include <memory>
#include<vector>
#include<array>
#include "../../common/Geometry.h"

class Controller;
class OriginalShader;
class MinCameraUI;
enum class ControllerType;

constexpr int MEMO_VERTS_NUM = 4;

enum class MoveState
{
    NONE,   //撮っていない
    STOP,   //硬直中
    SHRINK, //左下に縮小
    DOWN,   //下に移動
    MAX
};

class MinCamera
{
public:
    MinCamera(ControllerType type);
    ~MinCamera();

    //初期化
    void Init(void);
    void InitMemoPosition(void);
    //更新
    void Update(bool IsCamera);
    //描画
    void Draw(void);

    bool GetIsOnCamera()const;
    void SetIsOnCamera(bool IsCamera);

    void MoveSub(float& pos ,float max , float value);
    void MoveAdd(float& pos, float max, float value);

    //フラッシュを押したかどうか処理
    bool GetFlashFlag(void);

    //レンダーターゲットセット
    void SetRenderTarget(int screenID);

    //写真ハンドル取得
    std::vector<int>& GetPhotoVector(void);
private:

    //カメラの位置、回転更新
    void MoveCamera(void);

    //フラッシュ処理
    void FlashCamera(void);

    //メモ処理
    void MemoProcess(void);

    //コントローラ情報
    std::unique_ptr<Controller> controller_;
    std::unique_ptr<MinCameraUI> minCameraUI_;  //プレイヤーのカプセル

    //シェーダ
    std::unique_ptr<OriginalShader> origShader_;

    //カメラを構えているか同課のフラグ
    bool onCamera_;

    //位置
    Vector3 pos_;
    //角度
    Vector3 angle_;

    bool flashOneFrameFlag_;//押した瞬間のみフラグ
    double stepWhiteTime_;  //画面が白い時間

    //カメラ画面情報
    std::unique_ptr<OriginalShader> cameraScreen_;   
    int psCameraHandle_;      //ピクセルシェーダ 
    int renderTarget_;        //レンダリングターゲット
    float whiteRange_;        //カメラフラッシュの白い部分の範囲
    float time_;              //時間

    //メモ写真情報
    std::unique_ptr<OriginalShader> memoScreen_;
    std::array<Vector3, MEMO_VERTS_NUM> memoPos_;//各頂点の位置
    std::vector<int> photoVector_;  //写真の情報保存先
    int psMemoHandle_;              //ピクセルシェーダ 
    int photoCount_;                //写真を撮った回数
    double stepMemoTime_;           //撮影時の写真硬直時間
    double stepMemoLerpTime_;       //補間時間
    double downAccel_;              //落下加速度
    MoveState moveState_;           //写真の状態
    MoveState oldMoveState_;        //一つ前の写真の状態
};
