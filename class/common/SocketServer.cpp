#include "SocketServer.h"
#include <stdio.h>
#include <string>
#include <sstream>
//#define _WINSOCK2API_
#include <WinSock2.h>// WinSocketを使用するためのInclude

#include <WS2tcpip.h>// WinSocketを使用するためのInclude
#include <DxLib.h>
#include "../object/Item/Item.h"
#include"../../_debug/_DebugConOut.h"

// WinSocketを使用するためのlibファイル
#pragma comment(lib, "ws2_32.lib")

SOCKET sock;	//ヘッダーに書いたらWinSock2で怒られたのでここに書いている


SocketServer::SocketServer()
{
}

SocketServer::~SocketServer()
{	
	CommunicationCutoff();
}

void SocketServer::InternetConnection()
{
	//自分のIPを取得
	IPDATA IPAddress[16];
	GetMyIPAddress(IPAddress);
	TRACE("IPアドレス　　%d.%d.%d.%d\n", IPAddress->d1, IPAddress->d2, IPAddress->d3, IPAddress->d4);
	TRACE("キー:W       数字:017\n");
	TRACE("キー:A       数字:030\n");
	TRACE("キー:S       数字:031\n");
	TRACE("キー:D       数字:032\n");
	TRACE("キー:LSHIFT  数字:042\n");
	TRACE("キー:TAB     数字:015\n");
	TRACE("キー:ESC     数字:001\n");
	TRACE("キー:Space   数字:057\n");
	TRACE("キー:E       数字:018\n");
	TRACE("左クリック   数字:101\n");
	TRACE("右クリック   数字:102\n");
	TRACE("２個同時押しは数字を足した数字\n");
	
	WSADATA wsaData;
	struct sockaddr_in sockAddr;

	// Winsockの初期化
	WSAStartup(MAKEWORD(2, 0), &wsaData);
	// ソケットを作成
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	// AF_INETを渡すことによりIPv4で通信する事を指定
	sockAddr.sin_family = AF_INET;
	// ポート番号を指定
	sockAddr.sin_port = htons(12345);
	// INADDR_ANYを指定してどのIPからでも通信を受け取る状態にする。
	sockAddr.sin_addr.S_un.S_addr = INADDR_ANY;
	// ソケットにアドレスを割り当てる
	bind(sock, (struct sockaddr*)&sockAddr, sizeof(sockAddr));
	// 接続ソケットを非同期モードにする
	u_long val = 1;
	ioctlsocket(sock, FIONBIO, &val);
}

bool SocketServer::Receive()
{

	char buf[256];
	int data;
	//配列の0リセット
	memset(buf, 0, sizeof(buf));
	//データの受け取り
	data = recv(sock, buf, sizeof(buf), 0);
	//受け取ったデータの確認
	if (data < 1)
	{
		if (WSAGetLastError() == WSAEWOULDBLOCK)
		{
			// データが来てない。
			cameraValue_.first = 0;
			cameraValue_.second = 0;
			mouseflag_ = false;
			keyValue_ = 0;
			supplementValue_ = false;
			itemId_ = ItemID::Non;
		}
	}
	else
	{
		//データが来た。
		//データが来たフラグをtrue
		IsConnection = true;

		std::stringstream dataStream;
		dataStream << buf;

		//データの格納
		TRACE(" x : ");
		SplitStorage(cameraValue_.first, dataStream);
		TRACE(" y : ");
		SplitStorage(cameraValue_.second, dataStream);
		TRACE(" falg : ");
		int mouseflagint;
		SplitStorage(mouseflagint, dataStream);
		TRACE(" key : ");
		int keyValue = 0;
		SplitStorage(keyValue, dataStream);
		TRACE("ダッシュ");
		int dashValue;
		SplitStorage(dashValue, dataStream);
		TRACE("アイテムID");
		int itemId;
		SplitStorage(itemId, dataStream);
		TRACE("インベントリ選択");
		int inventorySelect;
		SplitStorage(inventorySelect, dataStream);
		TRACE("\n\n");

		keyValue_ = keyValue;
		mouseflag_ = mouseflagint;
		supplementValue_ = dashValue;
		itemId_ = static_cast<ItemID>(itemId);
		inventorySelect_ = std::to_string(inventorySelect);
	}

	return true;

}

void SocketServer::CommunicationCutoff()
{
	// ソケット通信を終了
	closesocket(sock);
	// Winsockを終了
	WSACleanup();
}

int SocketServer::GetKeyValue()const
{
	return keyValue_;
}

int SocketServer::GetSupplementValue()const
{
	return supplementValue_;
}

ItemID SocketServer::GetItemID()const
{
	return itemId_;
}

std::string SocketServer::GetinventorySelect() const
{
	return inventorySelect_;
}

bool SocketServer::GetIsConnection() const
{
	return IsConnection;
}

bool SocketServer::GetMouseflag()const
{
	return mouseflag_;
}

CameraValue SocketServer::GetCameraValue()const
{
	return cameraValue_;
}

void SocketServer::SplitStorage(int& value, std::stringstream& dataStream)
{
	std::string numstr;
	std::getline(dataStream, numstr, ',');
	value = atoi(numstr.c_str());
	TRACE(" data %d ", value);
}

