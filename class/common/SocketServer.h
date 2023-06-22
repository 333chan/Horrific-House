#pragma once
#include <sstream>


enum class ItemID;
using CameraValue = std::pair<int, int>;
#define lpSocketServer SocketServer::GetInstance()
class SocketServer
{
public:
	static SocketServer& GetInstance()
	{
		static SocketServer s_Instance;
		return s_Instance;
	}
	//通信に接続
	void InternetConnection();
	//通信相手からの値を出力
	bool Receive();
	//通信を切る
	void CommunicationCutoff();

	//取得関数
	CameraValue GetCameraValue()const;
	//マウスの通信が着てるか取得
	bool GetMouseflag()const;
	//キー移動入力取得
	int GetKeyValue()const;
	//移動以外のキー入力取得
	int GetSupplementValue()const;
	//携帯のアイテムのID取得
	ItemID GetItemID()const;
	//インベントリの選択取得
	std::string GetinventorySelect()const;
	//データが一回でも来たか
	bool GetIsConnection()const;
private:
	SocketServer();
	~SocketServer();
	/// <summary>
	/// 文字列を一文字ずつ取り出してintを代入する
	/// </summary>
	/// <param name="value">代入する変数</param>
	/// <param name="dataStream">文字列</param>
	void SplitStorage(int& value, std::stringstream& dataStream);
	//通信からもらったカメラの移動値
	CameraValue cameraValue_;		
	//マウスの通信が着てるか
	bool mouseflag_;
	//キー移動入力
	int keyValue_;
	//移動以外のキー入力
	int supplementValue_;
	//携帯のアイテムのID
	ItemID itemId_;
	//インベントリの選択
	std::string inventorySelect_;
	//データが一回でも来たか
	bool IsConnection;
};

