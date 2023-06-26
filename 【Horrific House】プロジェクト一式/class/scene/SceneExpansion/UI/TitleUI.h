#pragma once
#include "UI.h"
#include "../../../common/Geometry.h"
#include "../../../tmx/TmxObj.h"
#include "../../../input/Controller.h"


enum class TitleColUI
{
	Non,
	Title,
	Play,
	Option,
	Exit,
	Mouse,
	Operation,
	Max
};
//ギミックオブジェクト情報
struct UIobj
{
	std::string objName_;   //オブジェクト名
	Vector3 pos_;           //座標
	Vector3 angle_;	//回転
	Vector3 scale_;
	UIobj(std::string objName, const Vector3& pos, const Vector3& angle, const Vector3& scale) :
		objName_(objName),
		pos_(pos),
		angle_(angle),
	    scale_(scale) {}
};
class OriginalShader;
class Controller;
enum class ControllerType;
class TitleUI :
	public UI
{
public:
	TitleUI();
	TitleUI(std::unique_ptr<Controller> controller);
	~TitleUI();
	void Init()override;
	void Update()override;
	void Draw()override;
	//クリックして当たったコリジョンのenumを返す
	TitleColUI GetHitUICol();
	//コントローラータイプを取得
	ControllerType GetControllerType();
	//現在のコントローラを取得
	std::unique_ptr<Controller> GetController();

	//プレタイトルが終わっていいか
	bool GetPreEndFlag();
private:
	//UIMapの初期化
	void InitUIMap();
	//クリックして当たったの
	void PutClickHit();	
	void LoadBinData(const char* bData,std::string pName);
	std::vector<UIobj> uiObjList_;
	//タイトルUIのマップ
	std::map<TitleColUI, UIObjWithModel<TitleColUI>> titleUIMap_;
	//決定当たったUI
	TitleColUI decideHitCol_;
	
	//モデルパス
	std::string modelPath_;
	//シェーダーハンドル
	int psLight_;
	int vsLight_;
protected:

	//コントローラ情報
	std::unique_ptr<Controller> controller_;
	//オリジナルシェーダー
	std::unique_ptr<OriginalShader> origShader_;
	std::unique_ptr<OriginalShader> originalShader_;
	//プレタイトル終わってるかフラグ
	bool PreEndFlag_;
};