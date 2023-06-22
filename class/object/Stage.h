#pragma once
#include "Obj.h"
//#include "gimmick/Gimmick.h"
#include "marker/ARmarker.h"
#include "../common/Geometry.h"

class Gimmick;
class DrawerFrame;
class Drawer;
class Door;
class LockDoor;
class InnerLockDoor;
class Item;
class Dial;
class DialFrame;
class DialLockDoor;
class HiddenText;
class FakeWall;
class Water;
class Book;

class OriginalShader;

struct ItemObj;
struct GimmickObj;
struct MarkerObj;

enum class ItemID;

class Stage :
    public Obj
{
public:
	Stage();
	~Stage();

	//初期化
	void Init(void) override;
	//破棄
	void Release(void) override;
	//更新
	void Update(void) override;
	//描画
	void Draw(void) override;
	void DrawDepth(bool minCameraFlg);
	void Draw(bool minCameraFlg, int depthRT);

	/// <summary>
	/// ギミックリストを返す
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	std::vector<std::shared_ptr<Gimmick>> GetGimmickList(void);

	/// <summary>
	/// アイテムリストを返す
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	std::vector<std::shared_ptr<Item>> GetItemList(void);


private:
	//担当.三ヶ島滉季(開始)---------------------------------------------------------------------------
	//データ読込処理まとめ
	void LoadObjData(void);

	//ギミックリスト追加処理まとめ
	void AddGimmick(void);
	//アイテムリスト追加処理まとめ
	void AddItem(void);
	/// <summary>
	/// string->wstring型に変換
	/// </summary>
	/// <param name="str"></param>
	/// <returns></returns>
	std::wstring ConvertToWstring(std::string& str);


	/// <summary>
	/// バイナリデータのすべてを読み込む
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <param name="格納したいリスト"></param>
	/// <param name="バイナリデータ名"></param>
	template<typename T>
	void LoadBinData(std::vector<T>& gList, const char* bData);

	/// <summary>
	/// バイナリデータの親オブジェクト以外を読み込む
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <param name="格納したいリスト"></param>
	/// <param name="バイナリデータ名"></param>
	/// <param name="親オブジェクト名"></param>
	template<typename T>
	void LoadBinData(std::vector<T>& gList, const char* bData,std::string pName);


	/// <summary>
	/// 移動アイテム用データ読込
	/// </summary>
	/// <param name="アイテムリスト"></param>
	/// <param name="配置データ"></param>
	/// <param name="移動後の配置データ"></param>
	/// <param name="親オブジェクト名"></param>
	void LoadBinData(std::vector<ItemObj>& iList, const char* bData1, const char* bData2, std::string pName);

	/// <summary>
	/// すべてのデータをギミックリスト追加
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <param name="ギミックリスト"></param>
	template<typename T>
	void AddGimmickList(std::vector<GimmickObj>& gList);

	/// <summary>
	/// 指定したオブジェクト名のデータのみギミックリスト追加
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <param name="ギミックリスト"></param>
	/// <param name="オブジェクト名"></param>
	template<typename T>
	void AddGimmickList(std::vector<GimmickObj>& gList, std::wstring objName);

	/// <summary>
	/// ギミックリスト追加
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <param name="ギミックリスト"></param>
	/// <param name="オブジェクト"></param>
	/// <param name="対応させたいアイテムID"></param>
	template<typename T>
	void AddGimmickList(std::vector<GimmickObj>& gList, std::wstring objName, ItemID itemID);

	/// <summary>
	/// ギミックリスト追加
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <param name="ギミックリスト"></param>
	/// <param name="オブジェクト"></param>
	/// <param name="指定したい法線の値"></param>
	template<typename T>
	void AddGimmickList(std::vector<GimmickObj>& gList, std::wstring objName, float normal);

	/// <summary>
	/// ギミックリスト追加
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <param name="ギミックリスト"></param>
	/// <param name="オブジェクト"></param>
	/// <param name="ダイアルの解除番号"></param>
	template<typename T>
	void AddGimmickList(std::vector<GimmickObj>& gList, std::wstring objName, std::string unlockNumber);

	/// <summary>
	/// マーカーリストに追加
	/// </summary>
	/// <param name="マーカーリスト"></param>
	/// <param name="オブジェクト名"></param>
	void AddMarkerList(std::vector<MarkerObj>& gList, std::wstring objName);
	
	/// <summary>
	/// アイテムリストの追加
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <param name="アイテムリスト"></param>
	template<typename T>
	void AddItemList(std::vector<ItemObj>& iList);

	//ギミック
	//すべてのデータ格納用リスト
	std::vector<GimmickObj> gimmickObjList_;

	//引き出しリスト
	std::vector<GimmickObj> drawerObjList_;

	//引き出し枠リスト
	std::vector<GimmickObj> drawerFrameObjList_;

	//ジュークボックスリスト
	std::vector<GimmickObj> jukeBoxObjList_;

	//使用するデータのみを格納するリスト
	std::vector<std::shared_ptr<Gimmick>> gimmickList_;

	//マーカー
	//すべてのデータ格納用リスト
	std::vector<MarkerObj> markerObjList_;
	//使用するデータのみを格納するリスト
	std::vector<std::shared_ptr<ARmarker>> markerList_;

	//アイテムリスト
	//鍵
	std::vector<ItemObj> keyObjList_;

	//写真
	std::vector<ItemObj> photObjList_;

	//メモ
	std::vector<ItemObj> memoObjList_;

	//本
	std::vector<ItemObj> bookObjList_;

	//使用するデータのみを格納するリスト
	std::vector<std::shared_ptr<Item>> itemList_;
	//担当.三ヶ島滉季(終了)---------------------------------------------------------------------------

	//オリジナルシェーダー
	std::unique_ptr<OriginalShader> origShader_;
	std::unique_ptr<OriginalShader> depthShader_;

	//モデルハンドル(stage)
	int stageHandle_;


};

