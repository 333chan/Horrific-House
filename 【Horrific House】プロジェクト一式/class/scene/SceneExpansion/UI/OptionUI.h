#pragma once
#include "UI.h"

enum class OptionColUI
{
    Non,
    Title,
    Control,
    Audio,
    Graphic,
    TitleReture,
    Reture,
    Mouse,
    Operation,
    Max
};

class Controller;
class OriginalShader;
enum class ControllerType;


class OptionUI:
    public UI
{
public:
    OptionUI(SceneID beforeSceneID);
    ~OptionUI();
    void Init()override;
    void Update()override;
    void Draw()override;
    //当たったUI
    OptionColUI GetAfterSceneCol();
private:

    //衝突判定
    void HitCollAndAssignProcess();

    /// <summary>
    /// クリックしたとき当たったか
    /// </summary>
    /// <param name="coll">四角形衝突判定</param>
    void HitCollClickAndAssignProcess(std::pair<std::string, Collision> coll);

    /// <summary>
    /// マウスが当たっているか
    /// </summary>
    /// <param name="coll">四角形衝突判定</param>
    std::pair<std::string, std::string> HitCollMouse(std::pair<std::string, Collision> coll);


    //関数ポインタ
    //ーーーーーーーーーーーーーーーーーーーーーー
    void (OptionUI::* UpdateFuncPtr_)();

    //更新関数ポインタに代入する関数
    void UpdateControl();         //キー説明
    void UpdateAudio();           //音量
    void UpdateGraphic();         //明るさ調整
    void UpdateToTitle();     // タイトルに行く
    void UpdateReturnScreen();    //戻る

    //描画関数ポインタ
    void (OptionUI::* DrawFuncPtr_)();

    //描画関数ポインタに代入する関数
    void DrawControl();         //キー説明
    void DrawAudio();           //音量
    void DrawGraphic();         //明るさ調整
    void DrawToTitle();     // タイトルに行く
    void DrawReturnScreen();    //戻る
    //ーーーーーーーーーーーーーーーーーーーーー

    //まとめたコリジョンのdraw
    void DrawColl();
    /// <summary>
    /// コリジョンのY軸を中心に描画
    /// </summary>
    /// <param name="coll">四角形衝突判定</param>
    /// <param name="imageStr">画像のハンドル用文字列</param>
    void DrawCollCenterY(std::pair<std::string, Collision> coll, std::string imageStr);

    /// <summary>
    /// コリジョンを中心に描画
    /// </summary>
    /// <param name="coll">四角形衝突判定</param>
    /// <param name="imageStr">画像のハンドル用文字列</param>
    void DrawCollCenter(std::pair<std::string, Collision> coll, std::string imageStr);


    /// <summary>
    /// SelectList専用描画
    /// </summary>
    /// <param name="coll">四角形衝突判定</param>
    void DrawSelectList(std::pair<std::string, Collision> coll);

    /// <summary>
    /// リストの描画
    /// </summary>
    /// <param name="coll">四角形衝突判定</param>
    /// <param name="maplist">描画に使うマップリスト</param>
    void DrawList(std::pair<std::string, Collision> coll, std::map<std::string, std::string> maplist);


    SceneID beforeSceneID_;      //前のシーンID
    UniqueScene returningScene_; //シーンを切り替える時用
    OptionColUI afterSceneID_;
    std::unique_ptr<Controller> controller_;//コントローラ情報

    //ファーストはtmxの文字列セカンドは処理のポインター
    std::map<std::string, void (OptionUI::*)()> selectMap_;//処理まとめリスト

    //std::map<std::string, std::string>* listToDraw_;        //描画するリストのポインタ

    //ファーストはtmxの文字列セカンドは画像の文字列
    //ーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーー
    std::map<std::string, std::string> displayMap_;        //描画だけリスト
    std::map<std::string, std::string> controlMap_;        //操作リスト
    std::map<std::string, std::string> audioMap_;          //音リスト
    std::map<std::string, std::string> graphicMap_;        //グラフィックリスト
    std::map<std::string, std::string> titleRetureMap_;    //タイトルに行くリスト
    std::map<std::string, std::string> retureMap_;         //戻るリスト

    std::map<std::string, std::string> supportMap_;
    //ーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーー

    std::pair<std::string, std::string> OldhitMouse_;       //今マウスが当たっているものを格納する場所
    std::pair<std::string, std::string> hitMouse_;       //今マウスが当たっているものを格納する場所


    std::string selection_; //今選択しているものを格納する場所


    int Volume_;    //音量
    float brightness_;//明るさ

    std::unique_ptr<OriginalShader> origShader_;

};

