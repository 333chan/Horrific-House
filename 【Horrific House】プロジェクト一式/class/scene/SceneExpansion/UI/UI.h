#pragma once
#include "../../../common/manager/ImageMng.h"
#include "../../../tmx/TmxObj.h"
#include "../../../common/CheckHit.h"
enum class StatusUI
{
	Stop,
	MoveIn,
	MoveOut,
	Max
};

enum class CntType
{
	Non,
	Key,
	Pad,
	Smh,
	Max
};

template <typename T>
struct UIObj
{
	std::string drawImage;
	std::pair<Vector2, Vector2> coll;
	std::string tipsImage;
	T CanHitUI;
};

template <typename T>
struct UISelectObj
{
	std::string drawImage;
	std::pair<Vector2, Vector2> coll;
	std::string tipsImage;
	std::string deviceImage;
	CntType type;
	T CanHitUI;
};

template <typename T>
struct UIObjWithModel
{
	std::string drawImage;
	std::pair<Vector2, Vector2> coll;
	std::string tipsImage;
	T CanHitUI;
	std::string modelStr;
};

struct MoveUIObj
{
	std::string imageName;
	Vector2 Destination;
	Vector2 Pos;
	StatusUI IsMove;
	int StopCnt;
	int popCnt;
	bool endflag;
};



class UI
{
public:
	UI();
	~UI();
	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
private:

protected:
	/*template<typename T>
	T PutClickHit(CollListName& collList,std::map<std::string,T> hitlist);*/
	
	
	/// <summary>
	/// マウスが当たった文字列を返す
	/// </summary>
	/// <param name="colllist">コリジョンのリスト</param>
	/// <returns>当たった文字列を返す</returns>
	std::string PutMouseHit(CollListName& colllist);
	/// <summary>
	/// Xを中心に合わせて描画
	/// </summary>
	/// <param name="coll">コリジョン</param>
	/// <param name="image">画像</param>
	void CollDrawCenterX(Collision coll, std::string image);
	/// <summary>
	/// 中心で描画
	/// </summary>
	/// <param name="coll">コリジョン</param>
	/// <param name="image">画像</param>
	void CollDrawCenter(Collision coll, std::string image);
	//マウス座標
	Vector2Int mousePos_;

	//tmx
	TmxObj tmxObj_;

	//マウス当たったUI
	std::string mouseHitCol_;
	//ヒント用coll
	std::pair<Vector2, Vector2> tipsColl_;
	//フェードインアウト関数
	double FadeInOutCnt_;
};